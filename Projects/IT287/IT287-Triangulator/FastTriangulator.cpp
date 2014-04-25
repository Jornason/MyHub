#include "FastTriangulator.h"
#include "Vector3D.h"
#include "Point3D.h"
using namespace geometry;
#include "Matrix4.h"
using namespace graphics;


FastTriangulator::FastTriangulator()
{
}

FastTriangulator::~FastTriangulator()
{

}

void FastTriangulator::triangulate(vector<Point3D>& vertices,vector<int>& polygons,vector<int>& length_polygons)
{
    int tris[1000][3];
    double **verts;  //VERTS[0] TO BE LEFT BLANK
    int countverts;
    unsigned int i,j;

    verts = new double*[1+polygons.size()];
    for (i=0;i<(1+polygons.size());i++)
    {
        verts[i] = new double[2];
    }

    //count how many total vertices are going to be passed to the triangulator. This is the sum of all vertices for all contours
    countverts = 0;
    for (i=0;i<length_polygons.size();i++)
        countverts += length_polygons[i];

    //first make sure this polygon is not facing + or - Z direction.
    //if it is, give a different set of 2D vertices (-z,y) instead of (x,y)

 /*   Vector3D a(vertices[polygons[0]],vertices[polygons[1]]);
    Vector3D b(vertices[polygons[0]],vertices[polygons[polygons.size()-1]]);
    Vector3D c = a * b;

    Vector3D z(0,0,1);
    Vector3D x(1,0,0);

    float dot = c.dot(z);

    vmap.clear();
    for (i=0;i<polygons.size();i++)
    {
        vmap[i+1] = polygons[i];
        if (fabs(c.dot(z))>0.01)
        {
            verts[i+1][0] = vertices[polygons[i]].getX();
            verts[i+1][1] = vertices[polygons[i]].getY();
        }
        else if (fabs(c.dot(x))>0.01)
        {
            verts[i+1][0] = -vertices[polygons[i]].getZ();
            verts[i+1][1] = vertices[polygons[i]].getY();
        }
        else
        {
            verts[i+1][0] = vertices[polygons[i]].getZ();
            verts[i+1][1] = vertices[polygons[i]].getX();
        }
    }
    */

    //form the local coordinate system of this polygon
    Vector3D X,Y,Z;

    Vector3D a(vertices[polygons[0]],vertices[polygons[1]]);
    Vector3D b(vertices[polygons[0]],vertices[polygons[polygons.size()-1]]);

    Z = a * b;
    Z.normalize();

    //compute X arbitrarily as perpendicular to Z
    X.init(Z[1]-Z[2],Z[2]-Z[0],Z[0]-Z[1]);
    X.normalize();

    Y = Z * X;
    Y.normalize();



    //form world to plane transformation, with first vertex as reference point
    Matrix4 trans(X[0],Y[0],Z[0],vertices[polygons[0]][0],X[1],Y[1],Z[1],vertices[polygons[0]][1],X[2],Y[2],Z[2],vertices[polygons[0]][2],0,0,0,1);
    trans = trans.inverse();

    vmap.clear();
    for (i=0;i<polygons.size();i++)
    {
        Point3D temp = vertices[polygons[i]];
        trans.multPos(temp.getAsArray());

        vmap[i+1] = polygons[i];
        verts[i+1][0] = temp[0];
        verts[i+1][1] = temp[1];
    }


    unsigned int numtriangles = gmt.triangulate_polygon(length_polygons.size(),&length_polygons[0],verts,tris);

    triangles.clear();
    for (i=0;i<numtriangles;i++)
    {
        for (j=0;j<3;j++)
            triangles.push_back(vmap[tris[i][j]]);
    }

    for (i=0;i<(1+polygons.size());i++)
    {
        delete []verts[i];
    }
    delete []verts;
}

void FastTriangulator::triangulate(vector<Point2D>& vertices,vector<int>& polygons,vector<int>& length_polygons)
{
    int tris[1000][3];
    double **verts;  //VERTS[0] TO BE LEFT BLANK
    int countverts;
    unsigned int i,j;

    verts = new double*[1+polygons.size()];
    for (i=0;i<(1+polygons.size());i++)
    {
        verts[i] = new double[2];
    }

    //count how many total vertices are going to be passed to the triangulator. This is the sum of all vertices for all contours
    countverts = 0;
    for (i=0;i<length_polygons.size();i++)
        countverts += length_polygons[i];

    //first make sure this polygon is not facing + or - Z direction.
    //if it is, give a different set of 2D vertices (-z,y) instead of (x,y)

 /*   Vector3D a(vertices[polygons[0]],vertices[polygons[1]]);
    Vector3D b(vertices[polygons[0]],vertices[polygons[polygons.size()-1]]);
    Vector3D c = a * b;

    Vector3D z(0,0,1);
    Vector3D x(1,0,0);

    float dot = c.dot(z);

    vmap.clear();
    for (i=0;i<polygons.size();i++)
    {
        vmap[i+1] = polygons[i];
        if (fabs(c.dot(z))>0.01)
        {
            verts[i+1][0] = vertices[polygons[i]].getX();
            verts[i+1][1] = vertices[polygons[i]].getY();
        }
        else if (fabs(c.dot(x))>0.01)
        {
            verts[i+1][0] = -vertices[polygons[i]].getZ();
            verts[i+1][1] = vertices[polygons[i]].getY();
        }
        else
        {
            verts[i+1][0] = vertices[polygons[i]].getZ();
            verts[i+1][1] = vertices[polygons[i]].getX();
        }
    }
    */



    vmap.clear();
    for (i=0;i<polygons.size();i++)
    {
        verts[i+1][0] = vertices[polygons[i]][0];
        verts[i+1][1] = vertices[polygons[i]][1];

        vmap[i+1] = polygons[i];
    }


    unsigned int numtriangles = gmt.triangulate_polygon(length_polygons.size(),&length_polygons[0],verts,tris);

    triangles.clear();
    for (i=0;i<numtriangles;i++)
    {
        for (j=0;j<3;j++)
            triangles.push_back(vmap[tris[i][j]]);
    }

    for (i=0;i<(1+polygons.size());i++)
    {
        delete []verts[i];
    }
    delete []verts;
}
