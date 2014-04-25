#ifndef FASTTRIANGULATOR_H
#define FASTTRIANGULATOR_H

#include <map>
using namespace std;


#include "GraphicsGemsTriangulator.h"


#include "Triangulator.h"
#include "Point2D.h"
using namespace geometry;

class FastTriangulator : public Triangulator
{
public:
    FastTriangulator();
    ~FastTriangulator();
    void triangulate(vector<Point3D>& vertices,vector<int>& polygons,vector<int>& length_polygons);
    void triangulate(vector<Point2D>& vertices,vector<int>& polygons,vector<int>& length_polygons);


private:
    GraphicsGemsTriangulator gmt;
    map<int,int> vmap; //key: vertex index in array of vertices passed to gmt, value: vertex index in the original vertex array passed to setPolygon function

};

#endif // FASTTRIANGULATOR_H
