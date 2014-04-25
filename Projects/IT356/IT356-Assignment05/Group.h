#ifndef GROUP_H
#define GROUP_H
#include "Windows.h"
#include <vector>
#include <GL/gl.h>
#include "Node.h"
using namespace std;
class Group: public Node
{
  protected:
    vector<Node*> children;
  public:
    Group(Scenegraph* graph, string name = "");
    ~Group();
    virtual void getLights(vector<Light>& lightvector, stack<glm::mat4>& modelView);
    virtual void draw(stack<glm::mat4>& modelView);
    virtual bool intersect(Ray& r, HitRecord& h, stack<glm::mat4>& modelview);
    void addChild(Node* child);
    const vector<Node*> getChildren() const;
    void makeNodemap(map<string, Node*>& nodemap);
};
#endif
