#include "Box.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#define max(A,B) A>B?A:B
Box::Box(Scenegraph* graph, string name)
  : Object(graph, name)
{
  int i, j, k, l;
  int DIM = 1;
  glm::mat4 faceTransforms[6];
  glm::mat4 identity = glm::mat4(1.0f);
  VertexAttribs v;
  glm::mat4 transform, inverseTranspose;
  faceTransforms[0] = glm::translate(identity, glm::vec3(-0.5f, 0.0f, 0.0f)) * glm::rotate(identity, 90.0f, glm::vec3(1, 0, 0)) * glm::rotate(identity, 90.0f, glm::vec3(0, 0, 1));
  faceTransforms[1] = glm::translate(identity, glm::vec3(0.5f, 0.0f, 0.0f)) * glm::rotate(identity, 90.0f, glm::vec3(1, 0, 0)) * glm::rotate(identity, -90.0f, glm::vec3(0, 0, 1));
  faceTransforms[2] = glm::translate(identity, glm::vec3(0.0f, 0.0f, -0.5f)) * glm::rotate(identity, -90.0f, glm::vec3(1, 0, 0));
  faceTransforms[3] = glm::translate(identity, glm::vec3(0.0f, 0.0f, 0.5f)) * glm::rotate(identity, 90.0f, glm::vec3(1, 0, 0));
  faceTransforms[4] = glm::translate(identity, glm::vec3(0.0f, -0.5f, 0.0f)) * glm::rotate(identity, 180.0f, glm::vec3(0, 0, 1));
  faceTransforms[5] = glm::translate(identity, glm::vec3(0.0f, 0.5f, 0.0f));
  for (k = 0; k < 6; k++)
  {
    transform = faceTransforms[k];
    inverseTranspose = glm::inverseTranspose(transform);
    for (i = 0; i < DIM + 1; i++)
    {
      for (j = 0; j < DIM + 1; j++)
      {
        glm::vec4 position, normal;
        position = glm::vec4(-0.5f + (1.0f * j / DIM), 0.0f, -0.5f + (1.0f * i / DIM), 1.0f);
        normal = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
        position = transform * position;
        normal = glm::normalize(inverseTranspose * normal);
        for (l = 0; l < 4; l++)
        {
          v.position[l] = position[l];
          v.normal[l] = normal[l];
        }
        v.texcoords[0] = (float)i / DIM;
        v.texcoords[1] = (float)j / DIM;
        vertexData.push_back(v);
      }
    }
    for (i = 0; i < DIM; i++)
    {
      for (j = 0; j < DIM; j++)
      {
        triangleIndices.push_back(k * (DIM + 1) * (DIM + 1) + i * (DIM + 1) + j);
        triangleIndices.push_back(k * (DIM + 1) * (DIM + 1) + i * (DIM + 1) + j + 1);
        triangleIndices.push_back(k * (DIM + 1) * (DIM + 1) + (i + 1) * (DIM + 1) + j + 1);
        triangleIndices.push_back(k * (DIM + 1) * (DIM + 1) + i * (DIM + 1) + j);
        triangleIndices.push_back(k * (DIM + 1) * (DIM + 1) + (i + 1) * (DIM + 1) + j + 1);
        triangleIndices.push_back(k * (DIM + 1) * (DIM + 1) + (i + 1) * (DIM + 1) + j);
      }
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
Box::~Box(void)
{
}
