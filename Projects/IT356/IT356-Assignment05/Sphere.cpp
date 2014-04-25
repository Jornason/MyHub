#include "Sphere.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <cmath>
#include <glm/gtc/swizzle.hpp>
#include <iostream>
Sphere::Sphere(Scenegraph* graph, string name)
  : Object(graph, name)
{
  float theta, phi;
  int STACKS = 100;
  int i, j;
  int SLICES = 100;
  float PI = 3.14159f;
  float cosphi, sinphi, costheta, sintheta;
  VertexAttribs v;
  for (i = 0; i <= STACKS; i++)
  {
    phi = -PI / 2 + i * PI / STACKS;
    cosphi = cos(phi);
    sinphi = sin(phi);
    for (j = 0; j <= SLICES; j++)
    {
      theta = 2 * j * PI / SLICES;
      costheta = cos(theta);
      sintheta = sin(theta);
      v.position[0] = cosphi * costheta;
      v.position[1] = sinphi;
      v.position[2] = -cosphi * sintheta;
      v.position[3] = 1;
      v.normal[0] = cosphi * costheta;
      v.normal[1] = sinphi;
      v.normal[2] = -cosphi * sintheta;
      v.normal[3] = 0;
      v.texcoords[0] = theta / (2 * PI);
      v.texcoords[1] = (phi + 0.5f * PI) / PI;
      vertexData.push_back(v);
    }
  }
  for (i = 0; i < STACKS; i++)
  {
    for (j = 0; j < SLICES; j++)
    {
      triangleIndices.push_back(i * (SLICES + 1) + j);
      triangleIndices.push_back(i * (SLICES + 1) + j + 1);
      triangleIndices.push_back((i + 1) * (SLICES + 1) + j + 1);
      triangleIndices.push_back(i * (SLICES + 1) + j);
      triangleIndices.push_back((i + 1) * (SLICES + 1) + j + 1);
      triangleIndices.push_back((i + 1) * (SLICES + 1) + j);
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
bool Sphere::intersect(Ray& r, HitRecord& h, stack<glm::mat4>& modelview)
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
  double a, b, c;
  a = glm::dot(objectRay.dir, objectRay.dir);
  b = 2 * glm::dot(objectRay.dir, objectRay.start);
  c = glm::dot(glm::vec3(glm::swizzle<glm::X, glm::Y, glm::Z>(objectRay.start)), glm::vec3(glm::swizzle<glm::X, glm::Y, glm::Z>(objectRay.start))) - 1;
  double disc = b * b - 4 * a * c;
  if (disc < 0)
    result = false;
  else
  {
    double t1, t2, t;
    t1 = (-b + sqrt(disc)) / (2 * a);
    t2 = (-b - sqrt(disc)) / (2 * a);
    if (t1 > t2)
    {
      double temp = t1;
      t1 = t2;
      t2 = temp;
    }
    if (t2 < 0)
      result = false;
    else
    {
      result = true;
      if (t1 < 0)
        t = t2;
      else
        t = t1;
      if ((h.time < 0) || (h.time > t))
      {
        glm::vec4 ptIntersect;
        ptIntersect = objectRay.start + objectRay.dir * (float)t;
        h.time = (float)t;
        h.point = objToView * ptIntersect;
        h.normal = glm::transpose(viewToObj) * glm::vec4(ptIntersect.x, ptIntersect.y, ptIntersect.z, 0);
        h.normal = glm::normalize(glm::vec4(h.normal.x, h.normal.y, h.normal.z, 0.0f));
        h.mat = material;
        GLfloat xTexture = 1.0f - ((glm::atan(ptIntersect.z, ptIntersect.x)) / 6.283185308f);
        GLfloat yTexture = 0.5f + ((glm::asin(ptIntersect.y) / 6.283185308f) * 2);
        h.textureCoordinates = glm::vec4(xTexture, yTexture, 0.0f, 1.0f);
        h.tex = this->getTexture();
      }
    }
  }
  modelview.pop();
  return result;
}
Sphere::~Sphere(void)
{
}
