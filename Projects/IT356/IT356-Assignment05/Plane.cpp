#include "Plane.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <cmath>
#include <glm/gtc/matrix_inverse.hpp>
#include <iostream>
#define max(A,B) A>B?A:B
Plane::Plane(Scenegraph* graph, string name)
  : Object(graph, name)
{
  int i, j;
  int DIM;
  VertexAttribs v;
  DIM = 1;
  for (i = 0; i < DIM + 1; i++)
  {
    for (j = 0; j < DIM + 1; j++)
    {
      v.position[0] = -0.5f + (1.0f * j / DIM);
      v.position[1] = 0.0f;
      v.position[2] = 0.5f - (1.0f * i / DIM);
      v.position[3] = 1.0f;
      v.normal[0] = 0.0f;
      v.normal[1] = 1.0f;
      v.normal[2] = 0.0f;
      v.normal[3] = 0.0f;
      v.texcoords[0] = (float)j / DIM;
      v.texcoords[1] = (float)i / DIM;
      vertexData.push_back(v);
    }
  }
  for (i = 0; i < DIM; i++)
  {
    for (j = 0; j < DIM; j++)
    {
      triangleIndices.push_back(i * (DIM + 1) + j);
      triangleIndices.push_back(i * (DIM + 1) + j + 1);
      triangleIndices.push_back((i + 1) * (DIM + 1) + j + 1);
      triangleIndices.push_back(i * (DIM + 1) + j);
      triangleIndices.push_back((i + 1) * (DIM + 1) + j + 1);
      triangleIndices.push_back((i + 1) * (DIM + 1) + j);
    }
  }
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[VertexBuffer]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VertexAttribs)*vertexData.size(), &vertexData[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[IndexBuffer]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*triangleIndices.size(), &triangleIndices[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[VertexBuffer]);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAttribs), BUFFER_OFFSET(0));
  glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAttribs), BUFFER_OFFSET(4 * sizeof(float)));
  glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAttribs), BUFFER_OFFSET(8 * sizeof(float)));
  glEnableVertexAttribArray(vPosition);
  glEnableVertexAttribArray(vNormal);
  glEnableVertexAttribArray(vTexCoord);
  glBindVertexArray(0);
  numIndices = triangleIndices.size();
  triangleIndices.clear();
  vertexData.clear();
}
bool Plane::intersect(Ray& r, HitRecord& h, stack<glm::mat4>& modelview)
{
  bool result;
  glm::mat4 objToView, viewToObj;
  result = false;
  modelview.push(modelview.top());
  modelview.top() = modelview.top() * animation * transform;
  objToView = modelview.top();
  viewToObj = glm::inverse(objToView);
  Ray objectRay;
  objectRay.start = viewToObj * r.start;
  objectRay.dir = viewToObj * r.dir;
  if (fabs(objectRay.dir.y) < 0.0001)   //ray parallel to the plane
    result = false;
  else
  {
    float t = -objectRay.start.y / objectRay.dir.y;
    if (t < 0)
      result = false;
    else
    {
      glm::vec4 ptIntersect;
      ptIntersect = objectRay.start + objectRay.dir * t;
      if ((ptIntersect.x < -0.5) || (ptIntersect.x > 0.5) || (ptIntersect.z < -0.5) || (ptIntersect.z > 0.5))
        result = false;
      else
      {
        result = true;
        if ((h.time < 0) || (h.time > t))
        {
          h.time = t;
          h.point = objToView * ptIntersect;
          h.normal = glm::transpose(viewToObj) * glm::vec4(0, 1, 0, 1);
          h.normal = glm::normalize(glm::vec4(h.normal.x, h.normal.y, h.normal.z, 0.0f));
          h.mat = material;
          GLfloat xTexture = ptIntersect.x + 0.5f;
          GLfloat yTexture = -ptIntersect.z + 0.5f;
          h.textureCoordinates = glm::vec4(xTexture, yTexture, 0.0f, 1.0f);
          h.tex = this->getTexture();
        }
      }
    }
  }
  modelview.pop();
  return result;
}
Plane::~Plane(void)
{
}
