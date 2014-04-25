#ifndef CONE_H_
#define CONE_H_
#include "Object.h"
class Cone: public Object
{
  public:
    Cone(Scenegraph* graph, string name = "");
    ~Cone();
};
#endif
