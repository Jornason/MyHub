#include "Object.h"
Object::Object(Scenegraph* graph, string name)
  : Node(graph, name)
{
  textureTransform = glm::mat4(1.0);
  texture = NULL;
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
  texture = NULL;
}
void Object::getLights(vector<Light>& lightvector, stack<glm::mat4>& modelView)
{
  modelView.push(modelView.top());
  modelView.top() = modelView.top() * animation * transform;
  for (unsigned int i = 0; i < lights.size(); i++)
  {
    lightvector.push_back(lights[i]);
    glm::vec4 l = lights[i].getPosition();
    l = modelView.top() * l;
    lightvector[lightvector.size() - 1].setPosition(l);
    if (lights[i].getSpotAngle() < 180)
    {
      glm::vec4 l = this->lights[i].getSpotDirection();
      l = modelView.top() * l;
      lightvector[lightvector.size() - 1].setSpotDirection(l);
    }
  }
  modelView.pop();
}
void Object::draw(stack<glm::mat4>& modelView)
{
  modelView.push(modelView.top());
  modelView.top() = modelView.top() * animation * transform;
  if (scenegraph != NULL)
  {
    glUniformMatrix4fv(scenegraph->modelviewLocation, 1, GL_FALSE, glm::value_ptr(modelView.top()));
    glUniformMatrix4fv(scenegraph->normalmatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(modelView.top()))));
    glUniformMatrix4fv(scenegraph->textureMatrixLocation, 1, GL_FALSE, glm::value_ptr(textureTransform));
    glUniform3fv(scenegraph->materialLocations.ambientLocation, 1, glm::value_ptr(material.getAmbient()));
    glUniform3fv(scenegraph->materialLocations.diffuseLocation, 1, glm::value_ptr(material.getDiffuse()));
    glUniform3fv(scenegraph->materialLocations.specularLocation, 1, glm::value_ptr(material.getSpecular()));
    glUniform1f(scenegraph->materialLocations.shininessLocation, material.getShininess());
  }
  if (texture != NULL)
    glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
  modelView.pop();
}
void Object::setMaterial(Material& m)
{
  material = m;
}
Material& Object::getMaterial()
{
  return material;
}
Texture* Object::getTexture()
{
  return texture;
}
void Object::setTexture(Texture* texture)
{
  this->texture = texture;
}
void Object::setTextureTransform(glm::mat4& mat)
{
  textureTransform = mat;
}
