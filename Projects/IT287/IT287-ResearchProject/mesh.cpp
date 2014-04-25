#include"mesh.h"
#include<fstream>
#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<queue>
#include<algorithm>
#include<thread>
using namespace std;

Mesh::Mesh(string name, string filename)
{
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glGenBuffers(NumBuffers, buffers);
  setName(name);
  //1. Read in an obj mesh file
  //2. Compute and transform it to the "best orientation" for the process
  //3. Cut the model into thin slices on the X-Z plane
  //4. Determine how stable every part of the model is
  //5. Adjust the stability model to consider the slices below each part
  //6. Print to a text file for debugging
  if (readInputMesh(filename))
  {
    cout << "mesh read" << endl;
  }
  else
  {
    cout << "mesh file read failed" << endl;
  }
  computeBestOrientation();
  createGraph();
  cutTheModelIntoSlices();
  cout << "cut into slices" << endl;
  computeStabilityModel();
  cout << "computed stability" << endl;
  adjustStability();
  cout << "adjusted stability" << endl;
  printSlices();
}

bool Mesh::readInputMesh(string& filenm)
{
  string line;
  const char* file = filenm.c_str();
  //Reset the stored vertex data and triangle mapping
  vertexData.clear();
  triangleIndices.clear();
  //Open the file
  ifstream ifile(file);
  if (!ifile.is_open())
  {
    cout << "Failed to open mesh file." << endl;
    return false;
  }
  while (getline(ifile, line))
  {
    stringstream linestr;
    string code;
    //Get the first token from each line
    linestr << line;
    linestr >> code;
    if (code == "f")
    {
      //Triangle face
      int a;
      //Consider alternative format for obj file
      if (line.find("//") != string::npos)
      {
        //Get the three vertices that make up this triangle
        linestr >> a;
        linestr.ignore(100, ' ');
        a--;
        triangleIndices.push_back(a);
        linestr >> a;
        linestr.ignore(100, ' ');
        a--;
        triangleIndices.push_back(a);
        linestr >> a;
        a--;
        triangleIndices.push_back(a);
      }
      else
      {
        //Get the three vertices that make up this triangle
        linestr >> a;
        a--;
        triangleIndices.push_back(a);
        linestr >> a;
        a--;
        triangleIndices.push_back(a);
        linestr >> a;
        a--;
        triangleIndices.push_back(a);
      }
    }
    else if (code == "v")
    {
      //Vertex
      VAttrs v;
      linestr >> v.x;
      linestr >> v.y;
      linestr >> v.z;
      v.w = 1;
      vertexData.push_back(v);
    }
    else
    {
      //Ignore mangled lines
      continue;
    }
  }
  ifile.close();
  return true;
}

void Mesh::computeBestOrientation()
{
  unsigned int i, j;
  //Find the centroid of the model (the average of all of the vertices)
  avgX = 0.0;
  avgY = 0.0;
  avgZ = 0.0;
  for (i = 0; i < vertexData.size(); i++)
  {
    avgX += vertexData[i].x;
    avgY += vertexData[i].y;
    avgZ += vertexData[i].z;
  }
  avgX /= vertexData.size();
  avgY /= vertexData.size();
  avgZ /= vertexData.size();
  Matrix3d mat;
  for (i = 0; i < 3; i++)
  {
    for (j = 0; j < 3; j++)
    {
      mat(i, j) = 0;
    }
  }
  double a;
  for (i = 0; i < vertexData.size(); i++)
  {
    a = (vertexData[i].x - avgX) * (vertexData[i].x - avgX) / vertexData.size();
    mat(0, 0) = mat(0, 0) + a;
    a = (vertexData[i].x - avgX) * (vertexData[i].y - avgY) / vertexData.size();
    mat(0, 1) = mat(0, 1) + a;
    a = (vertexData[i].x - avgX) * (vertexData[i].z - avgZ) / vertexData.size();
    mat(0, 2) = mat(0, 2) + a;
    a = (vertexData[i].y - avgY) * (vertexData[i].x - avgX) / vertexData.size();
    mat(1, 0) = mat(1, 0) + a;
    a = (vertexData[i].y - avgY) * (vertexData[i].y - avgY) / vertexData.size();
    mat(1, 1) = mat(1, 1) + a;
    a = (vertexData[i].y - avgY) * (vertexData[i].z - avgZ) / vertexData.size();
    mat(1, 2) = mat(1, 2) + a;
    a = (vertexData[i].z - avgZ) * (vertexData[i].x - avgX) / vertexData.size();
    mat(2, 0) = mat(2, 0) + a;
    a = (vertexData[i].z - avgZ) * (vertexData[i].y - avgY) / vertexData.size();
    mat(2, 1) = mat(2, 1) + a;
    a = (vertexData[i].z - avgZ) * (vertexData[i].z - avgZ) / vertexData.size();
    mat(2, 2) = mat(2, 2) + a;
  }
  SelfAdjointEigenSolver<Matrix3d> es;
  es.compute(mat);
  int largest = 0;
  int middle = 0;
  int smallest = 0;
  for (i = 0; i < 3; i++)
  {
    if (es.eigenvalues()[i] > es.eigenvalues()[largest])
    {
      largest = i;
    }
  }
  for (i = 0; i < 3; i++)
  {
    if (es.eigenvalues()[i] < es.eigenvalues()[smallest])
    {
      smallest = i;
    }
  }
  switch (largest)
  {
    case 0:
      switch (smallest)
      {
        case 1:
          middle = 2;
          break;
        case 2:
          middle = 1;
          break;
      }
      break;
    case 1:
      switch (smallest)
      {
        case 0:
          middle = 2;
          break;
        case 2:
          middle = 0;
          break;
      }
      break;
    case 2:
      switch (smallest)
      {
        case 1:
          middle = 0;
          break;
        case 0:
          middle = 1;
          break;
      }
      break;
  }
  eigenLargest = es.eigenvectors().col(largest);
  eigenMiddle = es.eigenvectors().col(middle);
  eigenSmallest = es.eigenvectors().col(smallest);
  glm::dmat4 rotationMatrix;
  double rot[16] =
  {
    eigenMiddle(0), eigenSmallest(0), eigenLargest(0), 0,
    eigenMiddle(1), eigenSmallest(1), eigenLargest(1), 0,
    eigenMiddle(2), eigenSmallest(2), eigenLargest(2), 0,
    1,              1,                1,               1
  };
  rotationMatrix = glm::make_mat4(rot);
  glm::dmat4 translationMatrix
    = glm::translate(glm::dmat4(1.0), glm::dvec3(-avgX, -avgY, -avgZ));
  glm::dmat4 transform = rotationMatrix * translationMatrix;
  for (i = 0; i < vertexData.size(); i++)
  {
    glm::dvec4 point
      = glm::dvec4(vertexData[i].x, vertexData[i].y, vertexData[i].z, 1.0);
    point = transform * point;
    vertexData[i].x = point[0];
    vertexData[i].y = point[1];
    vertexData[i].z = point[2];
  }
}

void Mesh::createGraph()
{
  //Efficiently map the triangles to vertices and edges in the model by using a
  //map-of-maps.
  //std::map<int, std::map<int, std::set<int>>>
  //          1         2   3             4
  //1. The key for the outer map:
  //   There is one of each vertex in the model
  //2. The inner map:
  //   This inner map holds all of the vertices that key vertex connects to (It
  //   therefore holds the edges.)
  //3. The key for the inner map also points to a vertex in the model.
  //   Remember that each edge is mapped in either direction
  //4. The inner mapped value is a set. The set contains indices that correspond
  //   to indices in the triangleIndices vertex. Each value points to the first
  //   of 3 vertices that make up a triangle (so each value will be a multiple
  //   of 3.
  for (unsigned int i = 0; i < triangleIndices.size(); i += 3)
  {
    std::map<int, std::set<int>> innerMaps[3];
    std::set<int> emptySet;
    //Add the first vertex of the triangle and connect it to the other 2
    //vertices of triangle
    innerMaps[0].insert(make_pair(triangleIndices[i + 1], emptySet));
    innerMaps[0].insert(make_pair(triangleIndices[i + 2], emptySet));
    graph.insert(make_pair(triangleIndices[i], innerMaps[0]));
    //Mark this edge as being contained in this triangle
    graph[triangleIndices[i]][triangleIndices[i + 1]].insert(i);
    graph[triangleIndices[i]][triangleIndices[i + 2]].insert(i);
    //Add the second vertex of the triangle and connect it to the other 2
    //vertices of triangle
    innerMaps[1].insert(make_pair(triangleIndices[i], emptySet));
    innerMaps[1].insert(make_pair(triangleIndices[i + 2], emptySet));
    graph.insert(make_pair(triangleIndices[i + 1], innerMaps[1]));
    //Mark this edge as being contained in this triangle
    graph[triangleIndices[i + 1]][triangleIndices[i]].insert(i);
    graph[triangleIndices[i + 1]][triangleIndices[i + 2]].insert(i);
    //Add the third vertex of the triangle and connect it to the other 2
    //vertices of triangle
    innerMaps[2].insert(make_pair(triangleIndices[i], emptySet));
    innerMaps[2].insert(make_pair(triangleIndices[i + 1], emptySet));
    graph.insert(make_pair(triangleIndices[i + 2], innerMaps[2]));
    //Mark this edge as being contained in this triangle
    graph[triangleIndices[i + 2]][triangleIndices[i]].insert(i);
    graph[triangleIndices[i + 2]][triangleIndices[i + 1]].insert(i);
  }
}

bool sortY(const VAttrs& lhs, const VAttrs& rhs)
{
  //This predicate is used as a comparator of vertices. They are compared by
  //their y value
  return lhs.y < rhs.y;
}

void Mesh::cutTheModelIntoSlices()
{
  //This method will cut the model into slices
  //1. The maximum and minimum y values in the model are found.
  //2. This is used to find the height of the model.
  //3. It is divided by 100 to determine the maximum thickness of a slice.
  //4. Sort the vertices by their y value.
  //5. Then, from the bottom of the model all the way to the top, start creating
  //   slices. If the y value between slices is too large, a slice will be
  //   inserted to cover the gap.
  //6. After a slice is created, check the graph to see if there are any parts
  //   of the graph that can be chopped off to speed up the search for the next
  //   slice.
  //7. At this point, reset the triangle sets in the graph that remains
  //8. Finally, increase the y value to find at what point the next slice will
  //   be created.
  //9. Return to step 5.
  double maximumYValue = vertexData[0].y;
  double minumumYValue = vertexData[0].y;
  unsigned int i;
  for (i = 1; i < vertexData.size(); i++)
  {
    if (vertexData[i].y > maximumYValue)
    {
      maximumYValue = vertexData[i].y;
    }
    if (vertexData[i].y < minumumYValue)
    {
      minumumYValue = vertexData[i].y;
    }
  }
  double heightOfModel = maximumYValue - minumumYValue;
  maximumThicknessOfSlice = heightOfModel / 100.0;
  sortedVertexData = vertexData;
  //Y values sorted here
  sort(sortedVertexData.begin(), sortedVertexData.end(), sortY);
  y = sortedVertexData[0].y;
  i = 0;
  //Keep a clean copy of the graph
  Graph savedGraph = graph;
  //There will be AT LEAST as many slices as there are UNIQUE Y VALUES in the
  //model
  while (i < sortedVertexData.size() - 1)
  {
    //Progress tracking
    cout << ++i << " out of " << sortedVertexData.size() << endl;
    //Create a slice
    createIndividualSlice();
    //Consume the graph
    cleanGraph(savedGraph);
    //Clean the remaining graph for the next slice
    graph = savedGraph;
    if (i == sortedVertexData.size() - 1)
    {
      //The next y value is equal to the y value of the final vertex
      y = sortedVertexData[i].y;
    }
    else if (y + maximumThicknessOfSlice <= sortedVertexData[i + 1].y)
    {
      //Insert a filler slice
      y += maximumThicknessOfSlice;
      i--;
    }
    else
    {
      //Create a slice at the y value of the next vertex
      y = sortedVertexData[i + 1].y;
    }
  }
}

void Mesh::createIndividualSlice()
{
  //Iterate through the graph that remains, looking an edge that straddles the
  //clipping plane
  GIter mainGraphIterator = graph.begin();
  for (; mainGraphIterator != graph.end(); mainGraphIterator++)
  {
    findStraddlingVertex(mainGraphIterator);
  }
}

void Mesh::findStraddlingVertex(GIter& mainGraphIterator)
{
  EList slice;
  int firstEdgeVertexIndex = (*mainGraphIterator).first;
  IMIter innerMapIterator = (*mainGraphIterator).second.begin();
  //Go through each of the connecting vertices to see if that staddle the
  //clipping plane with this vertex
  while (innerMapIterator != (*mainGraphIterator).second.end())
  {
    int secondEdgeVertexIndex = (*innerMapIterator).first;
    double v1Y = vertexData[firstEdgeVertexIndex].y;
    double v2Y = vertexData[secondEdgeVertexIndex].y;
    if ((v1Y >= y && v2Y < y) || (v1Y < y && v2Y >= y))
    {
      //We have found an edge that straddles. From here, walk along the surface
      //of the model, traveling along straddling edges. Eventually, you will
      //return to where you started. You now have all of the edges needed to
      //create a slice
      clipAlongYPlane(slice, firstEdgeVertexIndex, secondEdgeVertexIndex);
      if (!slice.empty())
      {
        //Find the exact points in this slices that straddle the clipping plane
        slicesData.push_back(getSliceVertices(slice));
        sliceVertexVec.push_back(slice);
      }
      break;
    }
    innerMapIterator++;
  }
}

void Mesh::clipAlongYPlane(EList& slice, int edgeVertex1, int edgeVertex2)
{
  //This is a recursive helper function for finding straddling edges in the
  //model.
  //We will never travel to a triangle twice. If we return to a triangle, we
  //will know that we are done
  TriSet triangleThatContainEdge = graph[edgeVertex1][edgeVertex2];
  if (triangleThatContainEdge.size() == 0)
  {
    //We have returned to the start. Exit.
    return;
  }
  //Save this edge in the slice
  slice.push_back(make_pair(edgeVertex1, edgeVertex2));
  //Find a triangle that contains this edge
  int t = *triangleThatContainEdge.begin();
  int nextEdgeVertex1, nextEdgeVertex2;
  //Find the other two edges that are in that triangle. One of them WILL
  //straddle the clipping plane
  nextSliceEdge(edgeVertex1, edgeVertex2, nextEdgeVertex1, nextEdgeVertex2, t);
  //Call the function recursively on THAT edge
  clipAlongYPlane(slice, nextEdgeVertex1, nextEdgeVertex2);
  return;
}

void Mesh::nextSliceEdge(int eV1, int eV2, int& nxtEV1, int& nxtEV2, int t)
{
  //Remove the triangle from consideration so we don't return to it
  graph[triangleIndices[t]][triangleIndices[t + 1]].erase(t);
  graph[triangleIndices[t]][triangleIndices[t + 2]].erase(t);
  graph[triangleIndices[t + 1]][triangleIndices[t]].erase(t);
  graph[triangleIndices[t + 1]][triangleIndices[t + 2]].erase(t);
  graph[triangleIndices[t + 2]][triangleIndices[t]].erase(t);
  graph[triangleIndices[t + 2]][triangleIndices[t + 1]].erase(t);
  //Find the other edge (that isn't this one) that also straddles the clipping
  //plane
  if (triangleIndices[t] != eV1 && triangleIndices[t] != eV2)
  {
    nxtEV2 = triangleIndices[t];
    double nxtEV2y = vertexData[nxtEV2].y;
    double eV1y = vertexData[eV1].y;
    if ((nxtEV2y >= y && eV1y < y) || (nxtEV2y < y && eV1y >= y))
    {
      nxtEV1 = eV1;
    }
    else
    {
      nxtEV1 = eV2;
    }
    return;
  }
  if (triangleIndices[t + 1] != eV1 && triangleIndices[t + 1] != eV2)
  {
    nxtEV2 = triangleIndices[t + 1];
    double nxtEV2y = vertexData[nxtEV2].y;
    double eV1y = vertexData[eV1].y;
    if ((nxtEV2y >= y && eV1y < y) || (nxtEV2y < y && eV1y >= y))
    {
      nxtEV1 = eV1;
    }
    else
    {
      nxtEV1 = eV2;
    }
    return;
  }
  if (triangleIndices[t + 2] != eV1 && triangleIndices[t + 2] != eV2)
  {
    nxtEV2 = triangleIndices[t + 2];
    double nxtEV2y = vertexData[nxtEV2].y;
    double eV1y = vertexData[eV1].y;
    if ((nxtEV2y >= y && eV1y < y) || (nxtEV2y < y && eV1y >= y))
    {
      nxtEV1 = eV1;
    }
    else
    {
      nxtEV1 = eV2;
    }
    return;
  }
}

vector<VAttrs> Mesh::getSliceVertices(EList& slice)
{
  //Given a list of vertex indices, get all of the coordinates for each vertex
  //and return them in a vector
  vector<VAttrs>sliceData;
  EIter sliceIter = slice.begin();
  while (sliceIter != slice.end())
  {
    VAttrs v1 = vertexData[(*sliceIter).first];
    VAttrs v2 = vertexData[(*sliceIter).second];
    double xDif = v1.x - v2.x;
    double yDif = v1.y - v2.y;
    double zDif = v1.z - v2.z;
    double yDif2 = v1.y - y;
    double ratio = yDif2 / yDif;
    VAttrs v3;
    v3.x = v1.x - (xDif * ratio);
    v3.y = y;
    v3.z = v1.z - (zDif * ratio);
    v3.w = 1;
    sliceData.push_back(v3);
    sliceIter++;
  }
  return sliceData;
}

void Mesh::cleanGraph(Graph& savedGraph)
{
  //Go through the graph and find edges that lie entirely below the current
  //clipping plane. These edges and vertices can be removed from the graph.
  GIter mainGraphIterator = savedGraph.begin();
  while (mainGraphIterator != savedGraph.end())
  {
    //We have the first vertex in the edge
    double firstVertexYVal = vertexData[(*mainGraphIterator).first].y;
    IMIter innerMapIterator = (*mainGraphIterator).second.begin();
    while (innerMapIterator != (*mainGraphIterator).second.end())
    {
      //We have the second vertex in the edge. Now find if they both are below
      //the y clipping plane
      double secondVertexYVal = vertexData[(*innerMapIterator).first].y;
      if (firstVertexYVal < y && secondVertexYVal < y)
      {
        //Erase the edge
        innerMapIterator = (*mainGraphIterator).second.erase(innerMapIterator);
      }
      else
      {
        innerMapIterator++;
      }
    }
    if ((*mainGraphIterator).second.empty())
    {
      //If we have erased all of the edges, erase the point as well
      mainGraphIterator = savedGraph.erase(mainGraphIterator);
    }
    else
    {
      mainGraphIterator++;
    }
  }
}

void Mesh::computeStabilityModel()
{
  unsigned int i;
  vector<PointStability>firstSlice;
  PointStability p;
  p.val = 0.0;
  p.sliceIndex = 0;
  p.support1 = -1;
  p.support2 = -1;
  for (i = 0; i < slicesData[0].size(); i++)
  {
    firstSlice.push_back(p);
  }
  stabilityVector.push_back(firstSlice);
  for (i = 1; i < slicesData.size(); i++)
  {
    vector<PointStability>stabilitySlice;
    for (unsigned int j = 0; j < slicesData[i].size(); j++)
    {
      PointStability q;
      q.val = 0.0;
      q.sliceIndex = -1;
      q.support1 = -1;
      q.support2 = -1;
      stabilitySlice.push_back(q);
    }
    stabilityVector.push_back(stabilitySlice);
  }
  static unsigned int maxThreads = thread::hardware_concurrency();
  if (maxThreads < 1)
  {
    maxThreads = 1;
  }
  vector<thread> t;
  for (i = 0; i < maxThreads; i++)
  {
    t.push_back(thread());
  }
  unsigned int numPerThread = slicesData.size() / maxThreads;
  unsigned int start;
  unsigned int end;
  for (i = 0; i < maxThreads - 1; i++)
  {
    start = (numPerThread * i);
    end = start + numPerThread;
    t[i] = thread(&Mesh::stabilityThread, this, start, end);
  }
  t[maxThreads - 1] = thread(&Mesh::stabilityThread, this, end,
                             slicesData.size());
  for (i = 0; i < maxThreads; i++)
  {
    t[i].join();
  }
}

void Mesh::stabilityThread(unsigned int start, unsigned int end)
{
  unsigned int i;
  for (i = start; i < end; i++)
  {
    double closestLowerY = findClosestLowerYValue(i);
    vector<pair<int, vector<VAttrs>>>lowerSlices;
    lowSlices(lowerSlices, closestLowerY);
    for (unsigned int j = 0; j < slicesData[i].size(); j++)
    {
      stabilityVector[i][j] = findBestStability(slicesData[i][j], lowerSlices);
    }
  }
}

double Mesh::findClosestLowerYValue(int i)
{
  double closestLowerY = slicesData[i][0].y;
  for (unsigned int j = 0; j < slicesData.size(); j++)
  {
    if (closestLowerY == slicesData[i][0].y)
    {
      if (slicesData[j][0].y < slicesData[i][0].y)
      {
        closestLowerY = slicesData[j][0].y;
      }
    }
    else
    {
      if (slicesData[i][0].y > slicesData[j][0].y)
      {
        double dif1 = slicesData[i][0].y - slicesData[j][0].y;
        double dif2 = slicesData[i][0].y - closestLowerY;
        if (dif1 < dif2)
        {
          closestLowerY = slicesData[j][0].y;
        }
      }
    }
  }
  return closestLowerY;
}

void Mesh::lowSlices(vector<pair<int, vector<VAttrs>>>& lowSlices, double lowY)
{
  for (unsigned int j = 0; j < slicesData.size(); j++)
  {
    if (slicesData[j][0].y == lowY)
    {
      lowSlices.push_back(make_pair(j, slicesData[j]));
    }
  }
}

PointStability Mesh::findBestStability(VAttrs& v, SliceVec& lowerSlices)
{
  PointStability bestStability;
  bool firstAttemptToFindBestStability = true;
  for (unsigned int i = 0; i < lowerSlices.size(); i++)
  {
    Perimeter slicePerimeter;
    fillPerimeter(slicePerimeter, lowerSlices[i].second, lowerSlices[i].first);
    vector<double>clippingPoints;
    clipPoints(slicePerimeter, clippingPoints, v.z);
    int count = countNumToSide(clippingPoints, v.x);
    PointStability distance;
    if (count % 2 != 0)
    {
      distance = findDistance(v, slicePerimeter);
      distance.val *= -1;
    }
    else
    {
      distance = findDistance(v, slicePerimeter);
    }
    if (firstAttemptToFindBestStability)
    {
      bestStability = distance;
      firstAttemptToFindBestStability = false;
    }
    else
    {
      if (bestStability.val > distance.val)
      {
        bestStability = distance;
      }
    }
  }
  return bestStability;
}

void Mesh::fillPerimeter(Perimeter& perim, vector<VAttrs>& lowerSlice, int i)
{
  for (unsigned int k = 0; k < lowerSlice.size() - 1; k++)
  {
    perim.second.push_back(make_pair(lowerSlice[k], lowerSlice[k + 1]));
  }
  pair<VAttrs, VAttrs>finalEdge;
  finalEdge = make_pair(lowerSlice[lowerSlice.size() - 1], lowerSlice[0]);
  perim.second.push_back(finalEdge);
  perim.first = i;
}

void Mesh::clipPoints(Perimeter& perim, vector<double>& clipPoints, double z)
{
  for (unsigned int k = 0; k < perim.second.size(); k++)
  {
    double firstZ = perim.second[k].first.z;
    double secondZ = perim.second[k].second.z;
    if ((firstZ >= z && secondZ < z) || (firstZ < z && secondZ >= z))
    {
      double xDif = perim.second[k].first.x - perim.second[k].second.x;
      double zDif = perim.second[k].first.z - perim.second[k].second.z;
      double zDif2 = perim.second[k].first.z - z;
      double ratio = zDif2 / zDif;
      clipPoints.push_back(perim.second[k].first.x - (xDif * ratio));
    }
  }
  sort(clipPoints.begin(), clipPoints.end());
}

int Mesh::countNumToSide(vector<double>& clippingPoints, double x)
{
  int count = 0;
  for (unsigned int k = 0; k < clippingPoints.size(); k++)
  {
    if (clippingPoints[k] < x)
    {
      count++;
    }
  }
  return count;
}

PointStability Mesh::findDistance(VAttrs& v, Perimeter& perimeter)
{
  double minEdgeDistance;
  double edgeDistance;
  unsigned int k;
  double xPt;
  double zPt;
  double xDif;
  double zDif;
  bool foundAnEdgeDistance = false;
  pair<int, pair<int, int>>lowerEdgeVerts = make_pair(-1, make_pair(-1, -1));
  for (k = 0; k < perimeter.second.size(); k++)
  {
    VAttrs v1 = perimeter.second[k].first;
    VAttrs v2 = perimeter.second[k].second;
    double xDirEdge = v1.x - v2.x;
    double zDirEdge = v1.z - v2.z;
    double xDirPerp = -zDirEdge;
    double zDirPerp = xDirEdge;
    double edgeSlope = xDirEdge / zDirEdge;
    double slopeBetweenEdgeFirstAndPt = (v1.x - v.x) / (v1.z - v.z);
    if (slopeBetweenEdgeFirstAndPt == edgeSlope)
    {
      if ((v1.x - v.x <= xDirEdge) && (v1.z - v.z <= zDirEdge))
      {
        edgeDistance = 0.0;
        foundAnEdgeDistance = true;
        if (k != perimeter.second.size() - 1)
        {
          lowerEdgeVerts = make_pair(perimeter.first, make_pair(k, k + 1));
        }
        else
        {
          lowerEdgeVerts = make_pair(perimeter.first, make_pair(k, 0));
        }
        break;
      }
    }
    else
    {
      bool intersect1 = false;
      bool intersect2 = false;
      double mag = sqrt(((xDirEdge) * (xDirEdge)) + (zDirEdge * zDirEdge));
      double newXEdge = xDirEdge / mag;
      double newZEdge = zDirEdge / mag;
      double newXP = xDirPerp / mag;
      double newZP = zDirPerp / mag;
      double den = ((newXP * newXEdge) - (newZP * newZEdge));
      double a1 = (newXEdge * (v1.z - v.z) + newZEdge * (v.x - v1.x)) / den;
      double b1 = ((v1.x - v.x) + (newXP * a1)) / newXEdge;
      double a2 = (newXEdge * (v2.z - v.z) + newZEdge * (v.x - v2.x)) / den;
      double b2 = ((v2.x - v.x) + (newXP * a2)) / newXEdge;
      if (a1 >= 0 && b1 >= 0 || a1 < 0 && b1 < 0)
      {
        intersect1 = true;
      }
      if (a2 >= 0 && b2 >= 0 || a2 < 0 && b2 < 0)
      {
        intersect2 = true;
      }
      if ((intersect1 && intersect2) || (!intersect1 && !intersect2))
      {
        continue;
      }
      else
      {
        double num;
        if (intersect1)
        {
          if (a1 >= 0)
          {
            num = (v2.z * -newXP - newZP * v.x - v.z * -newXP + newZP * v2.x);
            den = ((-newXEdge * -newZP) + newZEdge * -newXP);
            double c = num / den;
            xPt = v2.x - newXEdge * c;
            zPt = v2.z - newZEdge * c;
            xDif = v.x - xPt;
            zDif = v.z - zPt;
            edgeDistance = sqrt(xDif * xDif + zDif * zDif);
            if (k != perimeter.second.size() - 1)
            {
              lowerEdgeVerts = make_pair(perimeter.first, make_pair(k, k + 1));
            }
            else
            {
              lowerEdgeVerts = make_pair(perimeter.first, make_pair(k, 0));
            }
            foundAnEdgeDistance = true;
          }
          else
          {
            num = (v2.z * -newXP + newZP * v.x - v.z * newXP - newZP * v2.x);
            den = ((-newXEdge * newZP) + newZEdge * newXP);
            double c = num / den;
            xPt = v2.x - newXEdge * c;
            zPt = v2.z - newZEdge * c;
            xDif = v.x - xPt;
            zDif = v.z - zPt;
            edgeDistance = sqrt(xDif * xDif + zDif * zDif);
            if (k != perimeter.second.size() - 1)
            {
              lowerEdgeVerts = make_pair(perimeter.first, make_pair(k, k + 1));
            }
            else
            {
              lowerEdgeVerts = make_pair(perimeter.first, make_pair(k, 0));
            }
            foundAnEdgeDistance = true;
          }
        }
        else
        {
          if (a2 >= 0)
          {
            num = (v1.z * -newXP - newZP * v.x - v.z * -newXP + newZP * v1.x);
            den = ((newXEdge * -newZP) - newZEdge * -newXP);
            double c = num / den;
            xPt = v1.x + newXEdge * c;
            zPt = v1.z + newZEdge * c;
            xDif = v.x - xPt;
            zDif = v.z - zPt;
            edgeDistance = sqrt(xDif * xDif + zDif * zDif);
            if (k != perimeter.second.size() - 1)
            {
              lowerEdgeVerts = make_pair(perimeter.first, make_pair(k, k + 1));
            }
            else
            {
              lowerEdgeVerts = make_pair(perimeter.first, make_pair(k, 0));
            }
            foundAnEdgeDistance = true;
          }
          else
          {
            num = (v1.z * -newXP + newZP * v.x - v.z * newXP - newZP * v1.x);
            den = ((newXEdge * newZP) - newZEdge * newXP);
            double c = num / den;
            xPt = v1.x + newXEdge * c;
            zPt = v1.z + newZEdge * c;
            xDif = v.x - xPt;
            zDif = v.z - zPt;
            edgeDistance = sqrt(xDif * xDif + zDif * zDif);
            if (k != perimeter.second.size() - 1)
            {
              lowerEdgeVerts = make_pair(perimeter.first, make_pair(k, k + 1));
            }
            else
            {
              lowerEdgeVerts = make_pair(perimeter.first, make_pair(k, 0));
            }
            foundAnEdgeDistance = true;
          }
        }
      }
    }
    if (!foundAnEdgeDistance)
    {
      minEdgeDistance = edgeDistance;
    }
    else
    {
      if (minEdgeDistance > edgeDistance)
      {
        minEdgeDistance = edgeDistance;
      }
    }
  }
  xDif = perimeter.second[0].first.x - v.x;
  zDif = perimeter.second[0].first.z - v.z;
  double pointDistance = sqrt(xDif * xDif + zDif * zDif);
  double dif;
  pair<int, pair<int, int>>lowerPointVerts = make_pair(perimeter.first,
                         make_pair(0, 0));
  for (k = 0; k < perimeter.second.size(); k++)
  {
    xDif = perimeter.second[k].first.x - v.x;
    zDif = perimeter.second[k].first.z - v.z;
    dif = sqrt(xDif * xDif + zDif * zDif);
    if (dif < pointDistance)
    {
      pointDistance = dif;
      lowerPointVerts = make_pair(perimeter.first, make_pair(k, k));
    }
  }
  PointStability p;
  if (!foundAnEdgeDistance)
  {
    p.val = pointDistance;
    p.sliceIndex = lowerPointVerts.first;
    p.support1 = lowerPointVerts.second.first;
    p.support2 = lowerPointVerts.second.second;
    return p;
  }
  else
  {
    if (edgeDistance < pointDistance)
    {
      p.val = edgeDistance;
      p.sliceIndex = lowerEdgeVerts.first;
      p.support1 = lowerEdgeVerts.second.first;
      p.support2 = lowerEdgeVerts.second.second;
    }
    else
    {
      p.val = pointDistance;
      p.sliceIndex = lowerPointVerts.first;
      p.support1 = lowerPointVerts.second.first;
      p.support2 = lowerPointVerts.second.second;
    }
    return p;
  }
}

void Mesh::adjustStability()
{
  unsigned int i, j;
  for (i = 1; i < slicesData.size(); i++)
  {
    for (j = 0; j < slicesData[i].size(); j++)
    {
      PointStability pStability = stabilityVector[i][j];
      PointStability lowerStability1 =
        stabilityVector[pStability.sliceIndex][pStability.support1];
      PointStability lowerStability2 =
        stabilityVector[pStability.sliceIndex][pStability.support2];
      if (lowerStability1.val >= lowerStability2.val)
      {
        if (lowerStability1.val >= pStability.val)
        {
          pStability.val = lowerStability1.val;
          stabilityVector[i][j] = pStability;
        }
        else
        {
          stabilityVector[i][j] = pStability;
        }
      }
      else if (lowerStability2.val >= lowerStability1.val)
      {
        if (lowerStability2.val >= pStability.val)
        {
          pStability.val = lowerStability2.val;
          stabilityVector[i][j] = lowerStability2;
        }
        else
        {
          stabilityVector[i][j] = pStability;
        }
      }
    }
  }
}

void Mesh::printSlices()
{
  ofstream outFile("slices.txt");
  if (outFile.is_open())
  {
    unsigned int i;
    for (i = 0; i < sliceVertexVec.size(); i++)
    {
      outFile << "slice " << i << " y=" << slicesData[i][0].y << endl;
      for (unsigned int j = 0; j < sliceVertexVec[i].size(); j++)
      {
        outFile << "vertex1" << sliceVertexVec[i][j].first;
        outFile << " vertex2" << sliceVertexVec[i][j].second;
        outFile << " x=" << slicesData[i][j].x;
        outFile << " z=" << slicesData[i][j].z;
        outFile << " stability=" << stabilityVector[i][j].val;
        outFile << " supporting slice=" << stabilityVector[i][j].sliceIndex;
        outFile << " supporting vertex1=" << stabilityVector[i][j].support1;
        outFile << " supporting vertex2=" << stabilityVector[i][j].support2;
        outFile << endl;
      }
    }
    outFile << endl;
    outFile.close();
  }
  else
  {
    cout << "Failed to open stability file for writing" << endl;
  }
}

Mesh::~Mesh(void)
{
  if (VAO != 0)
  {
    glDeleteBuffers(NumBuffers, buffers);
    glDeleteVertexArrays(1, &VAO);
  }
}

void Mesh::setTransform(glm::mat4& obj)
{
  transform.push(obj);
}

glm::mat4 Mesh::getTransform()
{
  return transform.top();
}

void Mesh::clearTransform()
{
  transform.pop();
}
void Mesh::setName(const string& name)
{
  this->name = name;
}
void Mesh::setColor(float r, float g, float b)
{
  color = glm::vec3(r, g, b);
}
glm::vec3 Mesh::getColor()
{
  return color;
}

void Mesh::passToGPU()
{
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[VertexBuffer]);
  size_t sizeOfVBuf = sizeof(VAttrs) * vertexData.size();
  glBufferData(GL_ARRAY_BUFFER, sizeOfVBuf, &vertexData[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[IndexBuffer]);
  size_t sizeOfTBuf = sizeof(GLuint) * triangleIndices.size();
  const GLvoid* data = &triangleIndices[0];
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfTBuf, data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[VertexBuffer]);
  glVertexAttribPointer(vPos, 4, GL_FLOAT, 0, sizeof(VAttrs), BUFSET(0));
  glEnableVertexAttribArray(vPos);
  glBindVertexArray(0);
}

void Mesh::draw(GLint objColLoc, double tolerance)
{
  for (unsigned int i = 0; i < slicesData.size(); i++)
  {
    drawSlice(i, objColLoc, tolerance);
  }
}

void Mesh::drawSlice(unsigned int i, GLint objColLoc, double tolerance)
{
  for (unsigned j = 0; j < slicesData[i].size(); j++)
  {
    glm::vec3 color;
    if (stabilityVector[i][j].val > tolerance)
    {
      color = glm::vec3(1, 0, 0);
    }
    else
    {
      color = glm::vec3(0, 1, 0);
    }
    glVertexAttrib3fv(objColLoc, glm::value_ptr(color));
    glBegin(GL_POINTS);
    VAttrs v = slicesData[i][j];
    glVertex3d(v.x, v.y, v.z);
    glEnd();
  }
}
