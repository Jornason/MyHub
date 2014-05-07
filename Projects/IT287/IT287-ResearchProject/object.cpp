#include "object.h"

Object::Object()
{
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glGenBuffers(NumBuffers, buffers);
}

Object::~Object()
{
  if (VAO != 0)
  {
    glDeleteBuffers(NumBuffers, buffers);
    glDeleteVertexArrays(1, &VAO);
  }
}

void Object::setTransform(glm::mat4& obj)
{
  transform.push(obj);
}

glm::mat4 Object::getTransform()
{
  return transform.top();
}

void Object::clearTransform()
{
  transform.pop();
}
void Object::setColor(float r, float g, float b)
{
  color = glm::vec3(r, g, b);
}
glm::vec3 Object::getColor()
{
  return color;
}
void Object::passToGPU()
{
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[VertexBuffer]);
  size_t sizeOfVBuf = sizeof(VertexAttributes) * vertexData.size();
  glBufferData(GL_ARRAY_BUFFER, sizeOfVBuf, &vertexData[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[IndexBuffer]);
  size_t sizeOfTBuf = sizeof(GLuint) * triangleIndices.size();
  const GLvoid* data = &triangleIndices[0];
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfTBuf, data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[VertexBuffer]);
  glVertexAttribPointer(vPos, 4, GL_FLOAT, 0, sizeof(VertexAttributes),
                        BUFSET(0));
  glEnableVertexAttribArray(vPos);
  glBindVertexArray(0);
}

void Object::draw(GLint, double)
{
  //not implemented
}
