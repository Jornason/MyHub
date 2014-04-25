#ifndef MESH_H_
#define MESH_H_
#include<GL/glew.h>
#include<GL/gl.h>
#include<vector>
#include<string>
#include<map>
#include<set>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<stack>
#include<list>
using namespace std;
#include "Eigen/EigenValues"
using namespace Eigen;
struct PointStability
{
  double val;
  int sliceIndex;
  int support1;
  int support2;
};
#define BUFSET(offset)((void*)(offset))
struct VAttrs
{
  double x;
  double y;
  double z;
  double w;
};
class Mesh
{
    typedef set<int>TriSet;
    typedef TriSet::iterator TriIter;
    typedef map<int, TriSet>InnerMap;
    typedef InnerMap::iterator IMIter;
    typedef std::map<int, InnerMap>Graph;
    typedef Graph::iterator GIter;
    typedef vector<std::pair<int, int>>EList;
    typedef EList::iterator EIter;
    typedef pair<int, vector<pair<VAttrs, VAttrs>>> Perimeter;
    typedef vector<pair<int, vector<VAttrs>>> SliceVec;
    typedef vector<VAttrs> VData;
  private:
    enum Buffer_IDs
    {
      VertexBuffer, IndexBuffer, NumBuffers
    };
    enum Attrib_IDs
    {
      vPos = 0
    };
    GLuint VAO;
    GLuint buffers[NumBuffers];
    vector<VAttrs>vertexData;
    vector<GLuint>triangleIndices;
    glm::vec3 color;
    stack<glm::mat4>transform;
    string name;
  public:
    void setTransform(glm::mat4& obj);
    glm::mat4 getTransform();
    void clearTransform();
    void setName(const string& name);
    void setColor(float r, float g, float b);
    glm::vec3 getColor();
    void passToGPU();
  private:
    Graph graph;
    vector<vector<VAttrs>>slicesData;
    vector<vector<PointStability>>stabilityVector;
    double maximumThicknessOfSlice;
    VData sortedVertexData;
    double y;
    vector<EList>sliceVertexVec;
    long total;
    double avgX;
    double avgY;
    double avgZ;
    Vector3d eigenLargest;
    Vector3d eigenMiddle;
    Vector3d eigenSmallest;
  private:
    void drawSlice(unsigned int i, GLint objColorLoc, double tolerance);
    bool readInputMesh(string& filenm);
    void computeBestOrientation();
    void createGraph();
    void cutTheModelIntoSlices();
    void createIndividualSlice();
    void findStraddlingVertex(GIter& mainGraphIter);
    void clipAlongYPlane(EList& slice, int edgeVertex1, int edgeVertex2);
    void nextSliceEdge(int eV1, int eV2, int& nxtEV1, int& nxtEV2, int t);
    vector<VAttrs> getSliceVertices(EList& slice);
    void cleanGraph(Graph& savedGraph);
    void computeStabilityModel();
    void stabilityThread(unsigned int start, unsigned int end);
    double findClosestLowerYValue(int i);
    void lowSlices(SliceVec& lowerSlices, double lowY);
    PointStability findBestStability(VAttrs& v, SliceVec& lowerSlices);
    void fillPerimeter(Perimeter& perim, VData& lowerSlice, int sliceIndex);
    void clipPoints(Perimeter& perim, vector<double>& clippingPoints, double z);
    int countNumToSide(vector<double>& clippingPoints, double x);
    PointStability findDistance(VAttrs& v, Perimeter& slicePerimeter);
    void adjustStability();
    void printSlices();
  public:
    Mesh(string name, string filename);
    ~Mesh(void);
    void draw(GLint objColLoc, double tolerance);
};
#endif
