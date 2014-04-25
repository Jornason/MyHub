#ifndef GGEMSTRIANGULATOR_H
#define GGEMSTRIANGULATOR_H

#include <sys/types.h>
#include <cstdlib>
//#include <memory.h>
#include <ctime>
#include <cmath>
#include <cstring>
#include <limits>



class GraphicsGemsTriangulator
{

    typedef struct {
      double x, y;
    } point_t, vector_t;

    /* Segment attributes */

    typedef struct {
      point_t v0, v1;		/* two endpoints */
      int is_inserted;		/* inserted in trapezoidation yet ? */
      int root0, root1;		/* root nodes in Q */
      int next;			/* Next logical segment */
      int prev;			/* Previous segment */
    } segment_t;

    typedef struct {
      int lseg, rseg;
      point_t hi, lo;
      int u0, u1;
      int d0, d1;
      int sink;
      int usave, uside;
      int state;
    } trap_t;

    typedef struct {
      int nodetype;
      int segnum;
      point_t yval;
      int trnum;
      int parent;
      int left, right;
    } node_t;

    typedef struct {
      int vnum;
      int next;
      int prev;
      int marked;			/* polygon */
    } monchain_t;

    typedef struct {
      point_t pt;
      int vnext[4];			/* next vertices for the 4 chains */
      int vpos[4];			/* position of v in the 4 chains */
      int nextfree;
    } vertexchain_t;

    struct global_s {
      int nseg;
    };

    #define MAX(a, b) (((a) > (b)) ? (a) : (b))
    #define MIN(a, b) (((a) < (b)) ? (a) : (b))

    #define CROSS(v0, v1, v2) (((v1).x - (v0).x)*((v2).y - (v0).y) - \
                               ((v1).y - (v0).y)*((v2).x - (v0).x))

    #define DOT(v0, v1) ((v0).x * (v1).x + (v0).y * (v1).y)

    #define MODULO_NEXT(v0, n) (((v0) - 1) % (n) + 1)

    #define FP_EQUAL(s, t) (fabs(s - t) <= C_EPS)

    #define CROSS_SINE(v0, v1) ((v0).x * (v1).y - (v1).x * (v0).y)
    #define LENGTH(v0) (sqrt((v0).x * (v0).x + (v0).y * (v0).y))
    #define INFINITY std::numeric_limits<double>::infinity()


private:
    const int T_X;
    const int T_Y;
    const int T_SINK;

    const int QSIZE;		/* maximum table sizes */
    const int TRSIZE;		/* max# trapezoids */
    const int SEGSIZE;		/* max# of segments */

    const int T_TRUE;
    const int T_FALSE;

    const int FIRSTPT;		/* checking whether pt. is inserted */
    const int LASTPT;

    const double EPS;

    const double C_EPS;

    const int S_LEFT;		/* for merge-direction */
    const int S_RIGHT;

    const int ST_VALID;		/* for trapezium table */
    const int ST_INVALID;

    const int SP_SIMPLE_LRUP;	/* for splitting trapezoids */
    const int SP_SIMPLE_LRDN;
    const int SP_2UP_2DN;
    const int SP_2UP_LEFT;
    const int SP_2UP_RIGHT;
    const int SP_2DN_LEFT;
    const int SP_2DN_RIGHT;
    const int SP_NOSPLIT;

    const int TR_FROM_UP;	/* for traverse-direction */
    const int TR_FROM_DN;

    const int TRI_LHS;
    const int TRI_RHS;
    const double LOG_2;

    struct global_s global;



    node_t *qs;		/* Query structure */
    trap_t *tr;		/* Trapezoid structure */
    segment_t *seg;		/* Segment table */
    int *permute;
    int choose_idx;

    int q_idx;
    int tr_idx;

    monchain_t *mchain;               /* Table to hold all the monotone */
                                      /* polygons . Each monotone polygon */
                                      /* is a circularly linked list */

    vertexchain_t *vert; /* chain init. information. This */
                                        /* is used to decide which */
                                        /* monotone polygon to split if */
                                        /* there are several other */
                                        /* polygons touching at the same */
                                        /* vertex  */

    int *mon;	/* contains position of any vertex in */
                                    /* the monotone chain for the polygon */
    int *visited;
    int chain_idx, op_idx, mon_idx;

private:
    int triangulate_single_polygon(int nvert, int posmax, int side, int op[][3]);
    int triangulate_monotone_polygons(int nvert, int nmonpoly, int op[][3]);
    int traverse_polygon(int mcur,int trnum,int from,int dir);
    int monotonate_trapezoids(int n);
    int make_new_monotone_poly(int mcur,int v0,int v1);
    int get_vertex_positions(int v0,int v1,int *ip,int *iq);
    double get_angle(point_t *vp0,point_t *vpnext,point_t *vp1);
    int inside_polygon(trap_t *t);
    int inserted(int segnum,int whichpt);
    int choose_segment();
    int construct_trapezoids(int nseg);
    int find_new_roots(int segnum);
    int add_segment(int segnum);
    int merge_trapezoids(int segnum,int tfirst,int tlast,int side);
    int locate_endpoint(point_t *v,point_t *vo,int r);
    int is_left_of(int segnum,point_t *v);
    int init_query_structure(int segnum);
    int newnode();
    /* Return a free trapezoid */
    int newtrap();
    int is_point_inside_polygon(double vertex[2]);
    int initialise(int nseg);
    int generate_random_ordering(int n);
    inline int _greater_than(point_t *v0, point_t *v1)
    {
      if (v0->y > v1->y + C_EPS)
        return T_TRUE;
      else if (v0->y < v1->y - C_EPS)
        return T_FALSE;
      else
        return (v0->x > v1->x);
    }


    inline int _equal_to(point_t *v0, point_t *v1)
    {
      return (FP_EQUAL(v0->y, v1->y) && FP_EQUAL(v0->x, v1->x));
    }

    inline int _greater_than_equal_to(point_t *v0, point_t *v1)
    {
      if (v0->y > v1->y + C_EPS)
        return T_TRUE;
      else if (v0->y < v1->y - C_EPS)
        return T_FALSE;
      else
        return (v0->x >= v1->x);
    }

    inline int _less_than(point_t *v0, point_t *v1)
    {
      if (v0->y < v1->y - C_EPS)
        return T_TRUE;
      else if (v0->y > v1->y + C_EPS)
        return T_FALSE;
      else
        return (v0->x < v1->x);
    }


    /* Return the maximum of the two points into the yval structure */
    inline int _max(point_t *yval, point_t *v0, point_t *v1)
    {
      if (v0->y > v1->y + C_EPS)
        *yval = *v0;
      else if (FP_EQUAL(v0->y, v1->y))
        {
          if (v0->x > v1->x + C_EPS)
            *yval = *v0;
          else
            *yval = *v1;
        }
      else
        *yval = *v1;

      return 0;
    }

    /* Return the minimum of the two points into the yval structure */
    inline int _min(point_t *yval, point_t *v0, point_t *v1)
    {
      if (v0->y < v1->y - C_EPS)
        *yval = *v0;
      else if (FP_EQUAL(v0->y, v1->y))
        {
          if (v0->x < v1->x)
            *yval = *v0;
          else
            *yval = *v1;
        }
      else
        *yval = *v1;

      return 0;
    }

    inline int is_collinear(int segnum,point_t *v,int is_swapped)
    {
      int n;

      /* First check if the endpoint is already inserted */
      if (!is_swapped)
        n = MODULO_NEXT(segnum + 1, global.nseg);
      else
        if ((n = segnum - 1) == 0)
          n = 1;

      return seg[n].is_inserted;
    }

    /* Get log*n for given n */
    inline int math_logstar_n(int n)
    {
      register int i;
      double v;

      for (i = 0, v = (double) n; v >= 1; i++)
        v = log((double)v)/LOG_2;

      return (i - 1);
    }


    inline int math_N(int n, int h)
    {
      register int i;
      double v;


      for (i = 0, v = (int) n; i < h; i++)
        v = log((double)v)/LOG_2;

      return (int) ceil((double) 1.0*n/v);
    }



    /* return a new mon structure from the table */
    inline int newmon()
    {
      return ++mon_idx;
    }


    /* return a new chain element from the table */
    inline int new_chain_element()
    {
      return ++chain_idx;
    }



public:
    GraphicsGemsTriangulator();
    int triangulate_polygon(int ncontours, const int cntr[], double **vertices, int (*triangles)[3]);
    ~GraphicsGemsTriangulator();
};

//========================================================================================

#endif // GRAPHICSGEMSTRIANGULATOR_H
