#ifndef PLANE_H_
#define PLANE_H_
#include "Object.h"
class Plane : public Object
{
  public:
    Plane(Scenegraph* graph, string name = "");
    bool intersect(Ray& r, HitRecord& h, stack<glm::mat4>& modelview);
    ~Plane();
};
#endif
