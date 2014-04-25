#ifndef POINT2D_H
#define POINT2D_H

#include <cmath>
#include <exception>

namespace geometry
{
    class Point2D
    {
        protected:
        double coord[2];
        public:
        Point2D() {coord[0] = coord[1] = 0;}
            Point2D(const Point2D& p){coord[0]=p.coord[0];coord[1]=p.coord[1];}
            Point2D(double x,double y) {this->coord[0]=x; this->coord[1]=y;}
            inline double getX() const
            {
                return coord[0];
            }

            inline void setX(double x)
            {
                this->coord[0] = x;
            }

            inline double getY() const
            {
                return coord[1];
            }

            inline void setY(double y)
            {
                this->coord[1] = y;
            }

            inline double getEuclideanDistance(const Point2D& d) const
            {
                return(sqrt((coord[0]-d.coord[0])*(coord[0]-d.coord[0]) + (coord[1]-d.coord[1])*(coord[1]-d.coord[1])));
            }

            inline void init(double x1,double y1)
            {
                this->coord[0]=x1;
                this->coord[1]=y1;
            }

            inline void init(const Point2D& a)
            {
                coord[0]=a.coord[0];
                coord[1]=a.coord[1];
            }

            inline const Point2D operator =(const Point2D& a)
            {
                coord[0] = a.coord[0];
                coord[1] = a.coord[1];
                return *this;
            }

            inline const Point2D operator +(const Point2D& p) const
            {
                double x1,y1;

                x1=coord[0]+p.coord[0];
                y1=coord[1]+p.coord[1];
                return Point2D(x1,y1);
            }

            inline const Point2D operator -(const Point2D& p) const
            {
                return Point2D(coord[0]-p.coord[0],coord[1]-p.coord[1]);
            }

            inline bool operator ==(const Point2D& p) const
            {
                if ((coord[0]==p.coord[0]) && (coord[1]==p.coord[1]))
                    return true;
                else
                    return false;
            }

            inline bool operator !=(Point2D p)
            {
                return (!((*this)==p));
            }

            inline double& operator [](int i) throw(int)
            {
                if (i==0)
                    return coord[0];
                if (i==1)
                    return coord[1];

                throw 1;
            }

            ~Point2D() {}
    };
}

#endif // POINT2D_H
