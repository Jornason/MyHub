#ifndef SPHERE_H_
#define SPHERE_H_
#include "Object.h"
class Sphere : public Object
{
  public:
    Sphere(Scenegraph* graph, string name = "");
    bool intersect(Ray& r, HitRecord& h, stack<glm::mat4>& modelview);
    ~Sphere();
};
#endif
