#ifndef BOX_H
#define BOX_H
#include "Object.h"
class Box : public Object
{
  public:
    Box(Scenegraph* graph, string name = "");
    ~Box();
};
#endif
