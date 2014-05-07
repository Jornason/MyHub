#include "object.h"

Object::Object()
{
  //Generate the VAO, bind it to this object, then generate the buffers
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glGenBuffers(NumBuffers, buffers);
}

Object::~Object()
{
  //If the vertex array object exists, delete the bufffers, then delete the VAO
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
  //Bind the VAO so all subsequent commands apply to this object
  glBindVertexArray(VAO);
  //Bind the vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, buffers[VertexBuffer]);
  //Reserve enough space for the vertexData
  glBufferData(GL_ARRAY_BUFFER, sizeof(VertexAttributes) * vertexData.size(), &vertexData[0], GL_STATIC_DRAW);
  //Bind the triangle index buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[IndexBuffer]);
  //Reserve enogh space the the riangle index data
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * triangleIndices.size(), &triangleIndices[0], GL_STATIC_DRAW);
  //Bind the vertex buffer again so commands apply to it again
  glBindBuffer(GL_ARRAY_BUFFER, buffers[VertexBuffer]);
  //Feed in the vertex data
  glVertexAttribPointer(vPos, 4, GL_FLOAT, 0, sizeof(VertexAttributes), BUFSET(0));
  //Enable the vertex data we have provided (position)
  glEnableVertexAttribArray(vPos);
  //Unbind this object
  glBindVertexArray(0);
}

void Object::draw(GLint, double)
{
  //not implemented
}
