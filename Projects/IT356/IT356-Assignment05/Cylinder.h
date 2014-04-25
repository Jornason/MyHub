#ifndef CYLINDER_H_
#define CYLINDER_H_
#include "Object.h"
class Cylinder: public Object
{
  public:
    Cylinder(Scenegraph* graph, string name = "", float inner_radius = 0.0f);
    ~Cylinder();
};
#endif
