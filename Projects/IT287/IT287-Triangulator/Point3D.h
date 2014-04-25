#ifndef POINT3D_H
#define POINT3D_H

#include <cmath>

namespace geometry
{
    class Point3D
    {
        protected:
        double coord[3];
        public:

            Point3D() {coord[0]=coord[1]=coord[2]=0;}
            Point3D(double x1,double y1,double z1) {init(x1,y1,z1);}
            Point3D(const Point3D& a) {init(a);}

            inline double getX() const{return coord[0];}
            inline double getY() const{return coord[1];}
            inline double getZ() const{return coord[2];}
            inline void setX(double x){this->coord[0] = x;}
            inline void setY(double y){this->coord[1] = y;}
            inline void setZ(double z){this->coord[2] = z;}

            inline double getEuclideanDistance(const Point3D& d) const
            {
                return(sqrt((coord[0]-d.coord[0])*(coord[0]-d.coord[0]) + (coord[1]-d.coord[1])*(coord[1]-d.coord[1]) + (coord[2]-d.coord[2])*(coord[2]-d.coord[2])));
            }
            inline void init(double x1,double y1,double z1)
            {
                this->coord[0]=x1;
                this->coord[1]=y1;
                this->coord[2]=z1;
            }

            inline void init(const Point3D& a)
            {
                coord[0]=a.coord[0];
                coord[1]=a.coord[1];
                coord[2]=a.coord[2];
            }

            inline const Point3D operator +(const Point3D& p) const
            {
                double x1,y1,z1;

                x1=coord[0]+p.coord[0];
                y1=coord[1]+p.coord[1];
                z1=coord[2]+p.coord[2];
                return Point3D(x1,y1,z1);
            }

            inline const Point3D operator -(const Point3D& p) const
            {
                double x1,y1,z1;

                x1=coord[0]-p.coord[0];
                y1=coord[1]-p.coord[1];
                z1=coord[2]-p.coord[2];
                return Point3D(x1,y1,z1);
            }

            inline const Point3D operator =(const Point3D& p)
            {
                init(p);
                return *this;
            }

            inline bool operator==(const Point3D& p) const
            {
                if ((coord[0]==p.coord[0]) && (coord[1]==p.coord[1]) && (coord[2]==p.coord[2]))
                        return true;
                else
                        return false;
            }

            inline bool operator !=(const Point3D& p) const
            {
                return (!(*this==p));
            }
            inline double& operator [](int i)
            {
                return coord[i];
            }

            inline const double& operator[](int i) const
            {
                return coord[i];
            }

            inline double *getAsArray()
            {
                return coord;
            }

            static inline const Point3D lerp(const Point3D& a,const Point3D& b,double t)
            {
                return Point3D(a.coord[0]+t*(b.coord[0]-a.coord[0]),a.coord[1]+t*(b.coord[1]-a.coord[1]),a.coord[2]+t*(b.coord[2]-a.coord[2]));
            }

            ~Point3D() {}
    };
}
#endif // POINT3D_H
