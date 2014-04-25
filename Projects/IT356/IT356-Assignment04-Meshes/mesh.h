#ifndef MESH_H_
#define MESH_H_
#include "Object.h"
class Mesh :public Object
{
public:
    string filename;
    Mesh(string name="", string filename="");
    ~Mesh(void);
};
#endif
