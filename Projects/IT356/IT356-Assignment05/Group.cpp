#include "Group.h"
Group::Group(Scenegraph* graph, string name)
  : Node(graph, name)
{
}
Group::~Group()
{
  for (unsigned int i = 0; i < children.size(); i++)
    delete children[i];
}
void Group::getLights(vector<Light>& lightvector, stack<glm::mat4>& modelView)
{
  modelView.push(modelView.top());
  modelView.top() = modelView.top() * animation * transform;
  for (unsigned int i = 0; i < lights.size(); i++)
  {
    lightvector.push_back(lights[i]);
    glm::vec4 l = this->lights[i].getPosition();
    l = modelView.top() * l;
    lightvector[lightvector.size() - 1].setPosition(l);
    if (lights[i].getSpotAngle() < 180)
    {
      glm::vec4 l = this->lights[i].getSpotDirection();
      l = modelView.top() * l;
      lightvector[lightvector.size() - 1].setSpotDirection(l);
    }
  }
  for (unsigned int i = 0; i < children.size(); i++)
    children[i]->getLights(lightvector, modelView);
  modelView.pop();
}
void Group::draw(stack<glm::mat4>& modelView)
{
  modelView.push(modelView.top());
  modelView.top() = modelView.top() * animation * transform;
  for (unsigned int i = 0; i < children.size(); i++)
    children[i]->draw(modelView);
  modelView.pop();
}
bool Group::intersect(Ray& r, HitRecord& h, stack<glm::mat4>& modelview)
{
  bool result = false;
  modelview.push(modelview.top());
  modelview.top() = modelview.top() * animation * transform;
  for (unsigned int i = 0; i < children.size(); i++)
  {
    bool tempresult;
    tempresult = children[i]->intersect(r, h, modelview);
    result = result || tempresult;
  }
  modelview.pop();
  return result;
}
void Group::addChild(Node* child)
{
  children.push_back(child);
}
const vector<Node*> Group::getChildren() const
{
  return children;
}
void Group::makeNodemap(map<string, Node*>& nodemap)
{
  Node::makeNodemap(nodemap);
  for (unsigned int i = 0; i < children.size(); i++)
    children[i]->makeNodemap(nodemap);
}
