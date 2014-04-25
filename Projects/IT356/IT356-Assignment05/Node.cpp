#include "Node.h"
Node::Node(Scenegraph* graph, string name)
{
  scenegraph = graph;
  transform = glm::mat4(1.0);
  animation = glm::mat4(1.0);
  this->name = name;
}
Node::~Node()
{
  lights.clear();
}
bool Node::intersect(Ray& r, HitRecord& h, stack<glm::mat4>& modelview)
{
  return false;
}
void Node::makeNodemap(map<string, Node*>& nodemap)
{
  if (name != "")
    nodemap[name] = this;
}
void Node::addLight(Light& l)
{
  lights.push_back(l);
}
void Node::setTransform(glm::mat4& obj)
{
  transform = obj;
}
void Node::setAnimationTransform(glm::mat4& mat)
{
  animation = mat;
}
void Node::setName(const string& name)
{
  this->name = name;
}
glm::mat4& Node::getTransform()
{
  return transform;
}
glm::mat4& Node::getAnimationTransform()
{
  return animation;
}
