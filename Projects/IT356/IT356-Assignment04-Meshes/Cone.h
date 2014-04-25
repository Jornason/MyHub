#ifndef CONE_H_
#define CONE_H_
#include "Object.h"
//represents a cone of unit radius and height 1 with the base along the X-Z plane and height along the +Y Axis
class Cone :public Object
{
public:
    Cone(string name="");
    ~Cone(void);
};
#endif
