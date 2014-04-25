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
Mesh::Mesh(string name, string filename, int orientation): Object(name)
{
  if (readInputMesh(filename, orientation))
  {
    cout << "mesh read orientation= " << orientation << endl;
  }
  else
  {
    cout << "mesh file read failed" << endl;
  }
  createGraph();
  cutTheModelIntoSlices();
  cout << "cut into slice" << endl;
  computeStabilityModel();
  cout << "computed stability" << endl;
  adjustStability();
  cout << "adjusted stability" << endl;
  printSlices();
}
bool Mesh::readInputMesh(string& filenm, int orientation)
{
  string line;
  vertexData.clear();
  triangleIndices.clear();
  const char* file = filenm.c_str();
  ifstream ifile(file);
  if (!ifile.is_open())
  {
    cout << "failed to open" << endl;
    return false;
  }
  while (getline(ifile, line))
  {
    stringstream linestr;
    linestr << line;
    string code;
    linestr >> code;
    if (code == "f")
    {
      int a;
      if (line.find("//") != string::npos)
      {
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
      VAttrs v;
      linestr >> v.x;
      linestr >> v.y;
      linestr >> v.z;
      v.w = 1;
      double temp;
      switch (orientation)
      {
        case 1:
          v.y *= -1.0;
          break;
        case 2:
          temp = v.y;
          v.y = v.z;
          v.z = temp;
          break;
        case 3:
          temp = v.y;
          v.y = v.z;
          v.z = temp;
          v.y *= -1.0;
          break;
        case 4:
          temp = v.y;
          v.y = v.x;
          v.x = temp;
          break;
        case 5:
          temp = v.y;
          v.y = v.x;
          v.x = temp;
          v.y *= -1.0;
          break;
      }
      vertexData.push_back(v);
    }
    else
    {
      continue;
    }
  }
  ifile.close();
  return true;
}
void Mesh::createGraph()
{
  for (unsigned int i = 0; i < triangleIndices.size(); i += 3)
  {
    InnerMap innerMaps[3];
    TriSet emptySet;
    innerMaps[0].insert(make_pair(triangleIndices[i + 1], emptySet));
    innerMaps[0].insert(make_pair(triangleIndices[i + 2], emptySet));
    graph.insert(make_pair(triangleIndices[i], innerMaps[0]));
    graph[triangleIndices[i]][triangleIndices[i + 1]].insert(i);
    graph[triangleIndices[i]][triangleIndices[i + 2]].insert(i);
    innerMaps[1].insert(make_pair(triangleIndices[i], emptySet));
    innerMaps[1].insert(make_pair(triangleIndices[i + 2], emptySet));
    graph.insert(make_pair(triangleIndices[i + 1], innerMaps[1]));
    graph[triangleIndices[i + 1]][triangleIndices[i]].insert(i);
    graph[triangleIndices[i + 1]][triangleIndices[i + 2]].insert(i);
    innerMaps[2].insert(make_pair(triangleIndices[i], emptySet));
    innerMaps[2].insert(make_pair(triangleIndices[i + 1], emptySet));
    graph.insert(make_pair(triangleIndices[i + 2], innerMaps[2]));
    graph[triangleIndices[i + 2]][triangleIndices[i]].insert(i);
    graph[triangleIndices[i + 2]][triangleIndices[i + 1]].insert(i);
  }
}
bool sortY(const VAttrs& lhs, const VAttrs& rhs)
{
  return lhs.y < rhs.y;
}
void Mesh::cutTheModelIntoSlices()
{
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
  sort(sortedVertexData.begin(), sortedVertexData.end(), sortY);
  y = sortedVertexData[0].y;
  i = 0;
  Graph savedGraph = graph;
  while (i < sortedVertexData.size() - 1)
  {
    cout << ++i << " out of " << sortedVertexData.size() << endl;
    createIndividualSlice();
    cleanGraph(savedGraph);
    graph = savedGraph;
    if (i == sortedVertexData.size() - 1)
    {
      y = sortedVertexData[i].y;
    }
    else if (y + maximumThicknessOfSlice <= sortedVertexData[i + 1].y)
    {
      y += maximumThicknessOfSlice;
    }
    else
    {
      y = sortedVertexData[i + 1].y;
    }
  }
}
void Mesh::createIndividualSlice()
{
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
  while (innerMapIterator != (*mainGraphIterator).second.end())
  {
    int secondEdgeVertexIndex = (*innerMapIterator).first;
    double v1Y = vertexData[firstEdgeVertexIndex].y;
    double v2Y = vertexData[secondEdgeVertexIndex].y;
    if ((v1Y >= y && v2Y < y) || (v1Y < y && v2Y >= y))
    {
      clipAlongYPlane(slice, firstEdgeVertexIndex, secondEdgeVertexIndex);
      if (!slice.empty())
      {
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
  TriSet triangleThatContainEdge = graph[edgeVertex1][edgeVertex2];
  if (triangleThatContainEdge.size() == 0)
  {
    return;
  }
  slice.push_back(make_pair(edgeVertex1, edgeVertex2));
  int t = *triangleThatContainEdge.begin();
  int nextEdgeVertex1, nextEdgeVertex2;
  nextSliceEdge(edgeVertex1, edgeVertex2, nextEdgeVertex1, nextEdgeVertex2, t);
  clipAlongYPlane(slice, nextEdgeVertex1, nextEdgeVertex2);
  return;
}
void Mesh::nextSliceEdge(int eV1, int eV2, int& nxtEV1, int& nxtEV2, int t)
{
  graph[triangleIndices[t]][triangleIndices[t + 1]].erase(t);
  graph[triangleIndices[t]][triangleIndices[t + 2]].erase(t);
  graph[triangleIndices[t + 1]][triangleIndices[t]].erase(t);
  graph[triangleIndices[t + 1]][triangleIndices[t + 2]].erase(t);
  graph[triangleIndices[t + 2]][triangleIndices[t]].erase(t);
  graph[triangleIndices[t + 2]][triangleIndices[t + 1]].erase(t);
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
  GIter mainGraphIterator = savedGraph.begin();
  while (mainGraphIterator != savedGraph.end())
  {
    double firstVertexYVal = vertexData[(*mainGraphIterator).first].y;
    IMIter innerMapIterator = (*mainGraphIterator).second.begin();
    while (innerMapIterator != (*mainGraphIterator).second.end())
    {
      double secondVertexYVal = vertexData[(*innerMapIterator).first].y;
      if (firstVertexYVal < y && secondVertexYVal < y)
      {
        (*mainGraphIterator).second.erase(innerMapIterator);
        innerMapIterator = (*mainGraphIterator).second.begin();
      }
      else
      {
        innerMapIterator++;
      }
    }
    if ((*mainGraphIterator).second.empty())
    {
      savedGraph.erase(mainGraphIterator);
      mainGraphIterator = savedGraph.begin();
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
    getLowerSlices(lowerSlices, closestLowerY);
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
void Mesh::getLowerSlices(vector<pair<int, vector<VAttrs>>>& lowerSlices,
                          double lowY)
{
  for (unsigned int j = 0; j < slicesData.size(); j++)
  {
    if (slicesData[j][0].y == lowY)
    {
      lowerSlices.push_back(make_pair(j, slicesData[j]));
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
void Mesh::fillPerimeter(Perimeter& perim,
                         vector<VAttrs>& lowerSlice, int sliceIndex)
{
  for (unsigned int k = 0; k < lowerSlice.size() - 1; k++)
  {
    perim.second.push_back(make_pair(lowerSlice[k], lowerSlice[k + 1]));
  }
  pair<VAttrs, VAttrs>finalEdge;
  finalEdge = make_pair(lowerSlice[lowerSlice.size() - 1], lowerSlice[0]);
  perim.second.push_back(finalEdge);
  perim.first = sliceIndex;
}
void Mesh::clipPoints(Perimeter& perim, vector<double>& clippingPoints,
                      double z)
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
      clippingPoints.push_back(perim.second[k].first.x - (xDif * ratio));
    }
  }
  sort(clippingPoints.begin(), clippingPoints.end());
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
  for (unsigned int i = 1; i < slicesData.size(); i++)
  {
    for (unsigned int j = 0; j < slicesData[i].size(); j++)
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
