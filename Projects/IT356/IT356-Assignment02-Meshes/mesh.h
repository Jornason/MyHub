#ifndef MESH_H_
#define MESH_H_
#include "Object.h"
class Mesh :public Object
{
public:
    Mesh(string name="", string filename="");
    ~Mesh(void);
};
#endif
