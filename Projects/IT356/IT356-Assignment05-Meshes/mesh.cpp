#include "mesh.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;
bool readInputMesh(string filename,vector<float>& positions,vector<int>& triangles);
Mesh::Mesh(Scenegraph *graph, string name, string filename)
    :Object(graph, name)
{
  vector<float> positions;
  vector<int> triangles;

  if(readInputMesh(filename,positions,triangles))
  {
    cout<<"mesh read"<<endl;
  }
  else
  {
    cout<<"mesh file read failed"<<endl;
  }
  VertexAttribs v;
  int i;
  for(i = 0; i < positions.size();)
  {
    v.position[0] = positions[i++];
    v.position[1] = positions[i++];
    v.position[2] = positions[i++];
    v.position[3] = 1;
    v.normal[0] = v.normal[1] = v.normal[2] = 1;
    v.normal[3] = 0;
    v.texcoords[0] = v.texcoords[1] = 1;
    vertexData.push_back(v);
  }
  for(i = 0; i < triangles.size();)
  {
    triangleIndices.push_back(triangles[i++]);
    triangleIndices.push_back(triangles[i++]);
    triangleIndices.push_back(triangles[i++]);
  }
  /*
   *Bind the VAO as the current VAO, so that all subsequent commands affect it
   */
  glBindVertexArray(VAO);

  /*
   *Allocate the VBO for vertex data and send it to the GPU
   */
  glBindBuffer(GL_ARRAY_BUFFER,buffers[VertexBuffer]);
  glBufferData(GL_ARRAY_BUFFER,sizeof(VertexAttribs)*vertexData.size(),&vertexData[0],GL_STATIC_DRAW);

  /*
   *Allocate the VBO for triangle indices and send it to GPU
   */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[IndexBuffer]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*triangleIndices.size(), &triangleIndices[0], GL_STATIC_DRAW);


  /*
   *Specify all the vertex attribute pointers, i.e. tell OpenGL how to organize data according to attributes rather than vertices
   */

  //first enable the correct VBO, since the "current" is the VBO for triangle indices
  glBindBuffer(GL_ARRAY_BUFFER,buffers[VertexBuffer]);

  //VertexData starts with position, so starting byte is 0
  glVertexAttribPointer(vPosition,4,GL_FLOAT,GL_FALSE,sizeof(VertexAttribs),BUFFER_OFFSET(0));
  //In VertexData, normal follows the position (4 floats), so start reading normals from 4*sizeof(float)
  glVertexAttribPointer(vNormal,4,GL_FLOAT,GL_FALSE,sizeof(VertexAttribs),BUFFER_OFFSET(4*sizeof(float)));
  //In VertexData, texture coordinates follow the position and normal (8 floats), so start reading texture coordinates from 8*sizeof(float)
  glVertexAttribPointer(vTexCoord,2,GL_FLOAT,GL_FALSE,sizeof(VertexAttribs),BUFFER_OFFSET(8*sizeof(float)));

  //enable the vertex attribute arrays
  glEnableVertexAttribArray(vPosition);
  glEnableVertexAttribArray(vNormal);
glEnableVertexAttribArray(vTexCoord);

  /*
   *Unbind the VAO to prevent accidental change to all the settings
   *so at this point, this VAO has two VBOs and two enabled VertexAttribPointers.
   * It is going to remember all of that!
   */
  glBindVertexArray(0);

  numIndices = triangleIndices.size();
  triangleIndices.clear();
  vertexData.clear();
}

Mesh::~Mesh(void)
{
}

bool readInputMesh(string filename,vector<float>& positions,vector<int>& triangles)
{
  int i,j;
  string line;
  positions.clear();
  triangles.clear();
  const char* file = filename.c_str();
  ifstream ifile(file);
  if (!ifile.is_open())
  {
    cout<<"failed to open"<<endl;
    return false;
  }
  while (getline(ifile,line))
  {
    stringstream linestr;
    linestr << line;
    string code;
    linestr >> code;
    if (code == "v")
    {
      //the next three are positions;
      float a;
      linestr >> a;
      positions.push_back(a);
      linestr >> a;
      positions.push_back(a);
      linestr >> a;
      positions.push_back(a);
    }
    else if (code == "f")
    {
      //the next three are triangle indices
      int a;
      linestr >> a;
      a--;
      triangles.push_back(a);
      linestr >> a;
      a--;
      triangles.push_back(a);
      linestr >> a;
      a--;
      triangles.push_back(a);
    }
    else
    {
      ifile.close();
      return false;
    }
  }
  ifile.close();
  return true;
}
