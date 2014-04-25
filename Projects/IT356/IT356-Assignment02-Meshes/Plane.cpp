#include "plane.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <cmath>
#define max(A,B) A>B?A:B
Plane::Plane(string name)
  :Object(name)
{
  int i,j;
	int DIM;
  VertexAttribs v;
  DIM = 1;
  for (i=0;i<DIM+1;i++)
  {
    for (j=0;j<DIM+1;j++)
    {
      v.position[0] = -0.5f+(1.0f*j/DIM);
      v.position[1] = 0.0f;
      v.position[2] = -0.5f + (1.0f*i/DIM);
      v.position[3] = 1.0f;
      v.normal[0] = 0.0f;
      v.normal[1] = 1.0f;
      v.normal[2] = 0.0f;
      v.normal[3] = 0.0f;
      v.texcoords[0] = (float)i/DIM;
      v.texcoords[1] = (float)j/DIM;
      vertexData.push_back(v);
    }
  }
  for (i=0;i<DIM;i++)
  {
    for (j=0;j<DIM;j++)
    {
      triangleIndices.push_back(i*(DIM+1)+j);
      triangleIndices.push_back(i*(DIM+1)+j+1);
      triangleIndices.push_back((i+1)*(DIM+1)+j+1);
      triangleIndices.push_back(i*(DIM+1)+j);
      triangleIndices.push_back((i+1)*(DIM+1)+j+1);
      triangleIndices.push_back((i+1)*(DIM+1)+j);
    }
  }
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER,buffers[VertexBuffer]);
  glBufferData(GL_ARRAY_BUFFER,sizeof(VertexAttribs)*vertexData.size(),&vertexData[0],GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[IndexBuffer]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*triangleIndices.size(), &triangleIndices[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER,buffers[VertexBuffer]);
  glVertexAttribPointer(vPosition,4,GL_FLOAT,GL_FALSE,sizeof(VertexAttribs),BUFFER_OFFSET(0));
  glVertexAttribPointer(vNormal,4,GL_FLOAT,GL_FALSE,sizeof(VertexAttribs),BUFFER_OFFSET(4*sizeof(float)));
  glVertexAttribPointer(vTexCoord,2,GL_FLOAT,GL_FALSE,sizeof(VertexAttribs),BUFFER_OFFSET(8*sizeof(float)));
  glEnableVertexAttribArray(vPosition);
  glEnableVertexAttribArray(vNormal);
  glEnableVertexAttribArray(vTexCoord);
  glBindVertexArray(0);
}
Plane::~Plane(void)
{
}
