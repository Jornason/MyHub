#ifndef MESH_H_
#define MESH_H_
#include"Object.h"
#include<list>
using namespace std;
struct PointStability
{
  double val;
  int sliceIndex;
  int support1;
  int support2;
};
class Mesh: public Object
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
    Graph graph;
    vector<vector<VAttrs>>slicesData;
    vector<vector<PointStability>>stabilityVector;
    double maximumThicknessOfSlice;
    VData sortedVertexData;
    double y;
    vector<EList>sliceVertexVec;
    long total;
  private:
    void drawSlice(unsigned int i, GLint objColorLoc, double tolerance);
    bool readInputMesh(string& filenm, int orientation);
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
    void getLowerSlices(SliceVec& lowerSlices, double lowY);
    PointStability findBestStability(VAttrs& v, SliceVec& lowerSlices);
    void fillPerimeter(Perimeter& perim, VData& lowerSlice, int sliceIndex);
    void clipPoints(Perimeter& perim, vector<double>& clippingPoints, double z);
    int countNumToSide(vector<double>& clippingPoints, double x);
    PointStability findDistance(VAttrs& v, Perimeter& slicePerimeter);
    void adjustStability();
    void printSlices();
  public:
    Mesh(string name, string filename, int orientation);
    ~Mesh(void);
    void draw(GLint objColLoc, double tolerance);
};
#endif
