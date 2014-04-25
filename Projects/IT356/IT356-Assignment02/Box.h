#ifndef BOX_H
#define BOX_H
#include "Object.h"
//a class that represents a box centered at origin with side 1
class Box : public Object
{
public:
    Box(string name="");
    virtual ~Box();
};
#endif
