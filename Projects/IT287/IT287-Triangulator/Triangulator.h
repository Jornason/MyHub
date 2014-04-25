#ifndef TRIANGULATOR_H
#define TRIANGULATOR_H

#include <vector>
using namespace std;
#include "Point3D.h"
using namespace geometry;

class Triangulator
{
public:
    Triangulator(){}
    virtual ~Triangulator(){}
    virtual void triangulate(vector<Point3D>& vertices,vector<int>& polygons,vector<int>& length_polygons)=0;
    vector<int> getTriangles()
    {
        return triangles;
    }

protected:
    vector<int> triangles;
};

#endif // TRIANGULATOR_H
