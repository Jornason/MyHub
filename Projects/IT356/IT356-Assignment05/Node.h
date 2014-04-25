#ifndef NODE_H
#define NODE_H
#include <string>
#include <map>
#include <stack>
#include <vector>
#include <glm/glm.hpp>
#include "Light.h"
#include "HitRecord.h"
#include "Ray.h"
using namespace std;
class Scenegraph;
class Node
{
  protected:
    string name;
    glm::mat4 transform, animation;
    Scenegraph* scenegraph;
    vector<Light> lights;
  public:
    Node(Scenegraph* graph, string name = "");
    virtual ~Node();
    virtual void draw(stack<glm::mat4>& modelView) = 0;
    virtual bool intersect(Ray& r, HitRecord& h, stack<glm::mat4>& modelview);
    virtual void makeNodemap(map<string, Node*>& nodemap);
    virtual void getLights(vector<Light>& lights, stack<glm::mat4>& modelView) = 0;
    void addLight(Light& l);
    void setTransform(glm::mat4& obj);
    void setAnimationTransform(glm::mat4& mat);
    void setName(const string& name);
    glm::mat4& getTransform();
    glm::mat4& getAnimationTransform();
};

#endif
