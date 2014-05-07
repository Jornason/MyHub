#ifndef MESH_H_
#define MESH_H_
#include<string>
#include<map>
#include<set>
#include<list>
#include"object.h"
using namespace std;
#include "Eigen/EigenValues"
using namespace Eigen;

class Mesh : public Object
{
    //This structure represents a vertex in the model and it's "stability". Also
    //stored are the crossSection that contains the vertex as well as the two
    //vertex indices in the slice directly below that support it
    struct PointStability
    {
      double stabilityValue;
      int containingSliceIndex;
      int supportingVertex1;
      int supportingVertex2;
      VertexAttributes vertexCoordinates;
    };
  private:

    //A map of maps that stores the connectivity of the vertices and edges in
    //the model
    //The out map's key is an index representing 1 vertex in the model. The
    //value of this corresponds to an index in the vertexData vector (see the
    //object class.
    //The out map's mapped value is an inner map that contains information about
    //all of the edges that connect to the key vertex
    //The inner map's key is also a vertex index (similar to the out map). It
    //represents a vertex that the outer map's key connects directly to.
    //The inner map's mapped value is a set of triangle indices. Each value
    //corresponds to an index in the triangleIndices vector (see the object
    //class. Each triangle represented is a triangle face in the model that
    //contains this edge. The actual number is index of the "first" vertex in
    //the triangle. This means each value should be a multple of 3. In addtion,
    //there should be exactly 2 values in the set for a well-formed 3D model
    //(since each edge can only be a part of two triangles).
    map<int, map<int, set<int>>> graph;

    //This vector contains all of the individual cross-sectional slices of the
    //3D model. Each slices is simply a series of vertices on a singular x-z
    //plane that when formed into a loop in the order their appear in the
    //vector, form a cross-sectional perimeter
    vector<vector<VertexAttributes>>modelCrossSectionData;

    //This vector contains the 4 stability models under consideration. The
    //program is trying to find the one that provides the best average stability
    //for each vertex.
    //Each stability model is similar to the modelCrossSectionData, except that
    //each value is a PointStability, instead of just VertexAttributes.
    vector<vector<PointStability>>stabilityModelsUnderConsideration;

    //Each stability model under consideration will be split into sub models
    vector<vector<Mesh>>subModels;

    //This is the best stability model find by the program. It's average
    //stability across all of the vertices is the best
    vector<vector<PointStability>>bestStabilityModel;

    //This stabilityModel only contains vertices that are in the ACTUAL model
    //(not filler vertices that only exist in slices)
    vector<PointStability>pairedDownModel;

    //This is the maximum distances allowed between cross-sectional slices
    double maximumThicknessOfSlice;

    //This is the vertex coordinate data sorted by y value
    vector<VertexAttributes> sortedVertexData;

    //This vector keeps track of how many slices have been computed for each
    //stability thread
    vector<int> progressCount;

    //This is the current y value under consideration during cross-section
    //creation
    double currentYValue;

    //These are the average X, Y, and Z values of all of the vertices in the
    //model
    double averageXValueInModel;
    double averageYValueInModel;
    double averageZValueInModel;

    //These are the 3 Eigen Vectors (stored by size)
    Vector3d eigenLargest;
    Vector3d eigenMiddle;
    Vector3d eigenSmallest;

  public:

    //Constructor for a mesh. Also computes stability for the mesh
    //Input: A file name for the .obj file containing model data
    Mesh(string filename);

  private:

    //Read an obj file in and store the vertex and triangle face data.
    //Input:  The obj's filename
    //Output: True if read was successful. Otherwise, false
    bool readInputMesh(string& filename);

    //Compute the Principle Component Analysis to get 3 mutually orthogonal
    //vectors to test stability with.
    void computePrincipalComponentAnalysis();

    //Rotation the model vertices to the new orientation
    //Input: a number representing one of 4 orientations with the largest eigen
    //vector parallel to the x-z plane
    void rotateModel(int orientation);

    //Create an adjacency graph based on the obj file
    void createGraph();

    //Cut the model into thin slices along the x-z planes
    void cutTheModelIntoSlices();

    //A helper method for cutting the model into slices
    void createIndividualSlice();

    //Find an edge in the graph that straddles the current x-z clipping plane
    //Input: An iterator for the outer map in the graph (we are checking each
    //edge connecting to a particular vertex
    void findStraddlingEdge(map<int, map<int, set<int>>>::iterator&
                              mainGraphIter);

    //Once a clipping edge has been found, this recursive method will walk along
    //the perimeter of the model traveling to the next edge that straddles the
    //clipping plane
    //Input: a reference to a slice that is being populated with vertices
    //Input: the first vertex in the current edge
    //Input: the secod vertex in the current edge
    void clipAlongYPlane(vector<std::pair<int, int>>& slice, int edgeVertex1,
                         int edgeVertex2);

    //A helper method for clipping along the x-z plane. Finds which edge in a
    //triangle to travel to next
    //Input: The first vertex in the edge we are currently on
    //Input: The second vertex in the edge we are currently on
    //Input: A reference to the first vertex in the edge to travel to (this will
    //be determined in this method)
    //Input: A reference to the second vertex in the edge to travel to (this
    //will be determined in this method)
    //Input: An index of the current triangle we are in
    void nextSliceEdge(int currentEdgeVertex1, int currentEdgeVertex2,
                       int& nextEdgeVertex1, int& nextEdgeVertex2, int triangle);

    //Once we have all of the vertex indices for a particular slice, find the
    //exact coordinates for each vertex in the cross-section by fining where the
    //straddling edge actually hits the x-z plane
    //Input: A vector of vertex pairs representing edges in the crosssectional
    //slice
    //Output: A vector of vertex coordinates representing the locations of each
    //vertex in the slice
    vector<VertexAttributes> getSliceVertices(vector<std::pair<int, int>>& slice);

    //Consume the graph. When an edge's two vertices are both lower than the
    //current x-z plane under consideration, remove them from the graph to speed
    //up the process of finding slices
    //Input: a graph to write to (reference)
    void cleanGraph(map<int, map<int, set<int>>>& savedGraph);

    //This is the primary method for computing the stability of each vertex in
    //the model
    void computeStabilityModel();

    //The task of computing the stability of each vertex is divided up among
    //threads. This method is called in a thread.
    //Input: The index for the first slice in the model that will be handled by
    //this thread
    //Input: The index of the last slice in the model that will be handled by
    //this thread
    void stabilityThread(unsigned int start, unsigned int end, unsigned int threadID);

    //This thread prints a readout to keep track of how many slices have been
    //completed by the stabilityThreads
    void progressThread();

    //Given a slice's y value, find the y value of the slice that is directly
    //below it. This will be equal to the y value of the vertex that is not
    //included in the slice itself, is below the current slice, and is closer
    //than any other vertex that meets the other two criteria.
    //Input: The index of the cross-sectional slice being considered
    //Output: The y value that is just below the current slice
    double findClosestLowerYValue(int sliceIndex);

    //Get all of the slices that match the y value that is just below the
    //current slice under consideration
    //Input: A refernce to a vector that will be filled with all of the lower
    //slices
    //Input: The yValue that all of these slices will share
    void lowSlices(vector<pair<int, vector<VertexAttributes>>>& lowerSlices,
                   double lowerYValue);

    //Find the best stability for each point in each slice by comparting it to
    //all of the slices that have a y value that is directly below it and
    //selecting the one that provides the best support
    //Input: The vertex that is under consideration
    //Input: The vector of slices that might support the vertex under
    //consideration
    //Output: The unadjusted stabibity for the input vertex (not adjusted to
    //consider whether vertices below support this one)
    PointStability findBestStability(VertexAttributes& vertex,
                                     vector<pair<int, vector<VertexAttributes>>>& lowerSlices);

    //Fill a perimeter loop around the model with vertex coordinate information
    //Input: A reference to a perimeter. This perimeter contains an integer that
    //represents the slice that is supporting this one as well as a series of
    //linked vertices to form a loop
    //Input: The slice being converted to linked edge
    //Input: The index of the supporting slice
    void fillPerimeter(pair<int, vector<pair<VertexAttributes, VertexAttributes>>>&
                       perimeter, vector<VertexAttributes>& lowerSlice, int supportingSliceIndex);

    //In order to find out if a point (if projected onto the slice below) is
    //inside or outside that slice (and therefore supported or not, this method
    //checks finds all of the points on a given y-z line that passes through the
    //the point tht also clip an edge in a slice. The x values of each point are
    //stored
    //Input: The perimeter of the slice being tested
    //Input: A reference to the vector holding the x values of place the line
    //clipped the perimeter
    //Input: the z value of the point being tested
    void clipPoints(pair<int, vector<pair<VertexAttributes, VertexAttributes>>>&
                    perimeter, vector<double>& clippingPoints, double z);

    //In conjuction the clipPoints, this method counts the number of clipping
    //points that are to one side of the vertex. If the number is odd, then the
    //vertex is supported by the slice below. Otherwise, it is outside and not
    //supported
    //Input: A vector containing the x values of the points on the slice
    //perimeter that clipped the line in clipPoints
    //Input: the x value of the vertex being tested
    //Output: the number of clip points on one side of the vertex
    int countNumToSide(vector<double>& clippingPoints, double x);

    //Find the distance that a point lies inside or outside of the slice below
    //it
    //Input: The vertex being tested
    //Input: The linked edges of the slice below
    //Output: A PointStability value that holds the distance inside or outside
    //the lower slice
    PointStability findDistance(VertexAttributes& vertex,
                                pair<int, vector<pair<VertexAttributes, VertexAttributes>>>& slicePerimeter);

    //Adjust the stability of each point in the model by considering if the the
    //points below are unstable, then this point is at least as unstable as the
    //points below. Otherwise, the stability is unchanged
    void adjustStability();

    //Cut the model up to alleviate instability
    void splitIntoSubModels();

    //Consider the 4 orientations computed and find the one that results in the
    //best average stability across all of the vertices
    //Input: A tolerance constant
    void findBestOrientation(double tolerance);

  public:

    //Destructor
    ~Mesh(void);

    //Draw the model point by point
    //Input: The object color's location
    //Input: The tolerance threshhold. Vertices with stability value less than
    //this are painted green. Otherwise, they are painted red
    void draw(GLint objectColorLocation, double tolerance);

  private:

    //Helper method for draw. Draws an individual slice of the model
    void drawSlice(unsigned int sliceIndex, GLint objectColorLocation,
                   double tolerance);
};
#endif
