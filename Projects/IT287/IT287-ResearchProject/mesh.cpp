#include"mesh.h"
#include<fstream>
#include<iostream>
#include<sstream>
#include<queue>
#include<algorithm>
#include<thread>
#include <chrono>
using namespace std;

Mesh::Mesh(string filename) : Object()
{
  //Mesh constructor
  //1. Read in an obj file and store the vertex coordinates in vertexData and the triangle face data in triangleIndices
  //2. Compute the Principle Component Analysis to find the 3 Eigen vectors
  //3. Find the four potential best orientations for the model by
  //   a. Rotating the model the the orientation
  //   b. Creating a graph of the model
  //   c. Creating cross sections of the model in this orientation
  //   d. Computing the stabilty of each point on its own
  //   e. Adjusting the stability to consider the points below it
  //4. Of the four orientations, find the one that had the best average stabilty across all of the points
  if (readInputMesh(filename))
  {
    cout << "mesh read" << endl;
  }
  else
  {
    cout << "mesh file read failed" << endl;
  }
  int orientation;
  cout << "Computing Principle Component Analysis" << endl;
  computePrincipalComponentAnalysis();
  for (orientation = 0; orientation < 4; orientation++)
  {
    cout << "Applying rotation of all model vertices for orientation " << orientation + 1 << " out of 4" << endl;
    rotateModel(orientation);
    cout << "Assembling graph for orientation " << orientation + 1 << " out of 4" << endl;
    createGraph();
    cout << "Cutting model into slices for orientation " << orientation + 1 << " out of 4" << endl;
    cutTheModelIntoSlices();
    cout << "Computing the raw stability model for orientation " << orientation + 1 << " out of 4" << endl;
    computeStabilityModel();
    cout << "Adjusting the raw stability model for orientation " << orientation + 1 << " out of 4" << endl;
    adjustStability();
  }
}

bool Mesh::readInputMesh(string& filename)
{
  //Read in an obj file to store the model
  //1. Clear any previous model data
  //2. Open the obj file as an input stream
  //3. Get each line of the file
  //   a. If the first character on the line is f, then the line represents a triangle face. Store the next 3 numbers in
  //      the triangle indices vector
  //   b. If the first character on the line is v, then the line represents a set of vertex coordinates. Store the
  //      vertex coordinates in vertexData
  //   c. Otherwise, ignore the line
  //4. Close the file
  string line;
  const char* file = filename.c_str();
  vertexData.clear();
  triangleIndices.clear();
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
    linestr << line;
    linestr >> code;
    if (code == "f")
    {
      //Triangle face
      int a;
      //Consider alternative format for obj file
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
      //Vertex
      VertexAttributes v;
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

void Mesh::computePrincipalComponentAnalysis()
{
  //Computer the PCA of the model
  //1. Find the centroid of the model by computing the average x, y, and z values of all of the vertices in the model
  //2. Initialize a 3D matrix to 0 for all values
  //3. Compute each value in the matrix. Each value is the sum across all vertice of the square of the distance along
  //   the respective axis that the point is from the respective axis average deivided by the number of vertices
  //4. Use a self adjoint eigen solver (part of the eigen library) to compute the 3 eigen vectors based on this matrix
  //   of sums
  //5. Arrange the eigen vectors by size
  unsigned int i, j;
  averageXVal = 0.0;
  averageYVal = 0.0;
  averageZVal = 0.0;
  for (i = 0; i < vertexData.size(); i++)
  {
    averageXVal += vertexData[i].x;
    averageYVal += vertexData[i].y;
    averageZVal += vertexData[i].z;
  }
  averageXVal /= vertexData.size();
  averageYVal /= vertexData.size();
  averageZVal /= vertexData.size();
  Matrix3d mat;
  for (i = 0; i < 3; i++)
  {
    for (j = 0; j < 3; j++)
    {
      mat(i, j) = 0;
    }
  }
  //Sum across all vertices
  for (i = 0; i < vertexData.size(); i++)
  {
    //(VertexAxisComponentValue - AverageAxisComponentValue)^2
    //--------------------------------------------------------
    //              number of vertices in model
    mat(0, 0) += (vertexData[i].x - averageXVal) * (vertexData[i].x - averageXVal) / vertexData.size();
    mat(0, 1) += (vertexData[i].x - averageXVal) * (vertexData[i].y - averageYVal) / vertexData.size();
    mat(0, 2) += (vertexData[i].x - averageXVal) * (vertexData[i].z - averageZVal) / vertexData.size();
    mat(1, 0) += (vertexData[i].y - averageYVal) * (vertexData[i].x - averageXVal) / vertexData.size();
    mat(1, 1) += (vertexData[i].y - averageYVal) * (vertexData[i].y - averageYVal) / vertexData.size();
    mat(1, 2) += (vertexData[i].y - averageYVal) * (vertexData[i].z - averageZVal) / vertexData.size();
    mat(2, 0) += (vertexData[i].z - averageZVal) * (vertexData[i].x - averageXVal) / vertexData.size();
    mat(2, 1) += (vertexData[i].z - averageZVal) * (vertexData[i].y - averageYVal) / vertexData.size();
    mat(2, 2) += (vertexData[i].z - averageZVal) * (vertexData[i].z - averageZVal) / vertexData.size();
  }
  SelfAdjointEigenSolver<Matrix3d> es;
  es.compute(mat);
  //Arrange the eigen vectors by size
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
}

void Mesh::rotateModel(int orientation)
{
  //Rotate the vertex data using the eigen vector data
  //1. Computing a rotation matrix based on the selected orientation. The largest Eigen value should always be parallel
  //   to the x-z plane
  //2. Compute a rotation matrix to rotate the model to the correct orientation
  //3. Compute a translation matrix to move each vertex to the centroid of the model
  //4. Transform each point in the model using the transformation matrices
  glm::dmat4 rotationMatrix;
  unsigned int i;
  if (orientation == 0)
  {
    double rot[16] =
    {
      eigenMiddle(0), eigenSmallest(0), eigenLargest(0), 0,
      eigenMiddle(1), eigenSmallest(1), eigenLargest(1), 0,
      eigenMiddle(2), eigenSmallest(2), eigenLargest(2), 0,
      1,              1,                1,               1
    };
    rotationMatrix = glm::make_mat4(rot);
  }
  else if (orientation == 1)
  {
    double rot[16] =
    {
      -eigenMiddle(0), -eigenSmallest(0), eigenLargest(0), 0,
      -eigenMiddle(1), -eigenSmallest(1), eigenLargest(1), 0,
      -eigenMiddle(2), -eigenSmallest(2), eigenLargest(2), 0,
      1,              1,                1,               1
    };
    rotationMatrix = glm::make_mat4(rot);
  }
  else if (orientation == 2)
  {
    double rot[16] =
    {
      eigenSmallest(0), -eigenMiddle(0), eigenLargest(0), 0,
      eigenSmallest(1), -eigenMiddle(1), eigenLargest(1), 0,
      eigenSmallest(2), -eigenMiddle(2), eigenLargest(2), 0,
      1,              1,                1,               1
    };
    rotationMatrix = glm::make_mat4(rot);
  }
  else
  {
    double rot[16] =
    {
      -eigenSmallest(0), eigenMiddle(0), eigenLargest(0), 0,
      -eigenSmallest(1), eigenMiddle(1), eigenLargest(1), 0,
      -eigenSmallest(2), eigenMiddle(2), eigenLargest(2), 0,
      1,              1,                1,               1
    };
    rotationMatrix = glm::make_mat4(rot);
  }
  glm::dmat4 translationMatrix = glm::translate(glm::dmat4(1.0), glm::dvec3(-averageXVal, -averageYVal, -averageZVal));
  glm::dmat4 transform = rotationMatrix * translationMatrix;
  for (i = 0; i < vertexData.size(); i++)
  {
    glm::dvec4 point = glm::dvec4(vertexData[i].x, vertexData[i].y, vertexData[i].z, 1.0);
    point = transform * point;
    vertexData[i].x = point[0];
    vertexData[i].y = point[1];
    vertexData[i].z = point[2];
  }
}

void Mesh::createGraph()
{
  //Efficiently map the triangles to vertices and edges in the model by using a map-of-maps.
  //std::map<int, std::map<int, std::set<int>>>
  //          1         2   3             4
  //1. The key for the outer map:
  //   There is one of each vertex in the model
  //2. The inner map:
  //   This inner map holds all of the vertices that key vertex connects to (It therefore holds the edges.)
  //3. The key for the inner map also points to a vertex in the model.
  //   Remember that each edge is mapped in either direction
  //4. The inner mapped value is a set. The set contains indices that correspond to indices in the triangleIndices
  //   vertex. Each value points to the first of 3 vertices that make up a triangle (so each value will be a multiple of
  //   3.
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

bool sortY(const Object::VertexAttributes& lhs, const Object::VertexAttributes& rhs)
{
  //This predicate is used as a comparator of vertices. They are compared by their y value
  return lhs.y < rhs.y;
}

void Mesh::cutTheModelIntoSlices()
{
  //This method will cut the model into slices
  //1. The maximum and minimum y values in the model are found.
  //2. This is used to find the height of the model.
  //3. It is divided by 100 to determine the maximum thickness of a slice.
  //4. Sort the vertices by their y value.
  //5. Then, from the bottom of the model all the way to the top, start creating slices. If the y value between slices
  //   is too large, a slice will be inserted to cover the gap.
  //6. After a slice is created, check the graph to see if there are any parts of the graph that can be chopped off to
  //   speed up the search for the next slice.
  //7. At this point, reset the triangle sets in the graph that remains
  //8. Finally, increase the y value to find at what point the next slice will be created.
  //9. Return to step 5.
  modelCrossSectionData.clear();
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
  currentYValue = sortedVertexData[0].y;
  i = 0;
  //Keep a clean copy of the graph
  map<int, map<int, set<int>>> savedGraph = graph;
  //There will be AT LEAST as many slices as there are UNIQUE Y VALUES in the model
  while (i < sortedVertexData.size() - 1)
  {
    //Progress tracking
    cout << ++i << " out of " << sortedVertexData.size() - 1;
    cout << '\r';
    //Create a slice
    createIndividualSlice();
    //Consume the graph
    cleanGraph(savedGraph);
    //Clean the remaining graph for the next slice
    graph = savedGraph;
    if (i == sortedVertexData.size() - 1)
    {
      //The next y value is equal to the y value of the final vertex
      currentYValue = sortedVertexData[i].y;
    }
    else if (currentYValue + maximumThicknessOfSlice <= sortedVertexData[i + 1].y)
    {
      //Insert a filler slice
      currentYValue += maximumThicknessOfSlice;
      i--;
    }
    else
    {
      //Create a slice at the y value of the next vertex
      currentYValue = sortedVertexData[i + 1].y;
    }
  }
  cout << endl;
}

void Mesh::createIndividualSlice()
{
  //Iterate through the graph that remains, looking an edge that straddles the clipping plane
  map<int, map<int, set<int>>>::iterator mainGraphIterator = graph.begin();
  for (; mainGraphIterator != graph.end(); mainGraphIterator++)
  {
    findStraddlingEdge(mainGraphIterator);
  }
}

void Mesh::findStraddlingEdge(map<int, map<int, set<int>>>::iterator& mainGraphIterator)
{
  //Starting from a point in the model, find if any of the connecting edges straddle the x-z clipping plane
  //1. For the key value in the outer map, iterate through the inner map.
  //2. Check each edge that is formed to see if one of the vertices is above the x-z clipping plane and one is below
  //3. If this is true, call a recursive method to start from this edge and find the other edge in the containing
  //   triangle that also clips the x-z plane
  //4. When the recursive method calls have all excited, we have traveled all the way around the model and created a
  //   cross section.
  //5. Get the exact coordinates for each point the clips the y-z plane and store it
  vector<std::pair<int, int>> slice;
  int firstEdgeVertexIndex = (*mainGraphIterator).first;
  map<int, set<int>>::iterator innerMapIterator = (*mainGraphIterator).second.begin();
  while (innerMapIterator != (*mainGraphIterator).second.end())
  {
    int secondEdgeVertexIndex = (*innerMapIterator).first;
    double vertex1YValue = vertexData[firstEdgeVertexIndex].y;
    double vertex2YValue = vertexData[secondEdgeVertexIndex].y;
    if ((vertex1YValue >= currentYValue && vertex2YValue < currentYValue)
        || (vertex1YValue < currentYValue && vertex2YValue >= currentYValue))
    {
      //We have found an edge that straddles. From here, walk along the surface of the model, traveling along straddling
      //edges. Eventually, you will return to where you started. You now have all of the edges needed to create a slice.
      clipAlongYPlane(slice, firstEdgeVertexIndex, secondEdgeVertexIndex);
      if (!slice.empty())
      {
        //Find the exact points in this slices that straddle the clipping plane
        modelCrossSectionData.push_back(getSliceVertices(slice));
      }
      break;
    }
    innerMapIterator++;
  }
}

void Mesh::clipAlongYPlane(vector<std::pair<int, int>>& slice, int currentEdgeVertex1, int currentEdgeVertex2)
{
  //Recursive helper for finding edges in the model that straddle the x-z plane
  //1. Get the triangle set that contains all of the triangles that contain this current edge
  //2. If no triangles could be found, then we have erased them all because we have already visited all of them on this
  //   slice. Exit the method
  //3. Otherwise, save the vertices in the slice
  //4. Choose a triangle from the previously mentioned set.
  //5. Call a method to find the other edge in that triangle that straddles the x-z clipping plane
  //6. Recursively call this method using THAT edge that you have now traveled to, building the slice as you go
  set<int> triangleThatContainEdge = graph[currentEdgeVertex1][currentEdgeVertex2];
  if (triangleThatContainEdge.size() == 0)
  {
    //We have returned to the start. Exit.
    return;
  }
  //Save this edge in the slice
  slice.push_back(make_pair(currentEdgeVertex1, currentEdgeVertex2));
  //Find a triangle that contains this edge
  int triangle = *triangleThatContainEdge.begin();
  int nextEdgeVertex1, nextEdgeVertex2;
  //Find the other two edges that are in that triangle. One of them WILL straddle the clipping plane
  nextSliceEdge(currentEdgeVertex1, currentEdgeVertex2, nextEdgeVertex1, nextEdgeVertex2, triangle);
  //Call the function recursively on THAT edge
  clipAlongYPlane(slice, nextEdgeVertex1, nextEdgeVertex2);
  return;
}

void Mesh::nextSliceEdge(int currentEdgeVertex1, int currentEdgeVertex2,int& nextEdgeVertex1, int& nextEdgeVertex2,
                         int triangle)
{
  //Find the next edge in a triangle to travel to
  //1. Erase the triangle we are in from the graph so we don't travel to it again during the creation of this slice
  //2. Check all of the other edges in this triangle to see which one also straddles the x-z clipping plane
  //3. Once the other straddling edge has been find, return by reference the vertices in this edge
  graph[triangleIndices[triangle]][triangleIndices[triangle + 1]].erase(triangle);
  graph[triangleIndices[triangle]][triangleIndices[triangle + 2]].erase(triangle);
  graph[triangleIndices[triangle + 1]][triangleIndices[triangle]].erase(triangle);
  graph[triangleIndices[triangle + 1]][triangleIndices[triangle + 2]].erase(triangle);
  graph[triangleIndices[triangle + 2]][triangleIndices[triangle]].erase(triangle);
  graph[triangleIndices[triangle + 2]][triangleIndices[triangle + 1]].erase(triangle);
  //Find the other edge (that isn't this one) that also straddles the clipping plane
  if (triangleIndices[triangle] != currentEdgeVertex1 && triangleIndices[triangle] != currentEdgeVertex2)
  {
    nextEdgeVertex2 = triangleIndices[triangle];
    double nxtEV2y = vertexData[nextEdgeVertex2].y;
    double eV1y = vertexData[currentEdgeVertex1].y;
    if ((nxtEV2y >= currentYValue && eV1y < currentYValue) || (nxtEV2y < currentYValue && eV1y >= currentYValue))
    {
      nextEdgeVertex1 = currentEdgeVertex1;
    }
    else
    {
      nextEdgeVertex1 = currentEdgeVertex2;
    }
    return;
  }
  if (triangleIndices[triangle + 1] != currentEdgeVertex1 && triangleIndices[triangle + 1] != currentEdgeVertex2)
  {
    nextEdgeVertex2 = triangleIndices[triangle + 1];
    double nxtEV2y = vertexData[nextEdgeVertex2].y;
    double eV1y = vertexData[currentEdgeVertex1].y;
    if ((nxtEV2y >= currentYValue && eV1y < currentYValue) || (nxtEV2y < currentYValue && eV1y >= currentYValue))
    {
      nextEdgeVertex1 = currentEdgeVertex1;
    }
    else
    {
      nextEdgeVertex1 = currentEdgeVertex2;
    }
    return;
  }
  if (triangleIndices[triangle + 2] != currentEdgeVertex1 && triangleIndices[triangle + 2] != currentEdgeVertex2)
  {
    nextEdgeVertex2 = triangleIndices[triangle + 2];
    double nxtEV2y = vertexData[nextEdgeVertex2].y;
    double eV1y = vertexData[currentEdgeVertex1].y;
    if ((nxtEV2y >= currentYValue && eV1y < currentYValue) || (nxtEV2y < currentYValue && eV1y >= currentYValue))
    {
      nextEdgeVertex1 = currentEdgeVertex1;
    }
    else
    {
      nextEdgeVertex1 = currentEdgeVertex2;
    }
    return;
  }
}

vector<Object::VertexAttributes> Mesh::getSliceVertices(vector<std::pair<int, int>>& slice)
{
  //Now that we have the edges straddle the clipping plane, find the exact points that these edges clip the plane. This
  //will form the slice
  //1. Iterate through all of the edges. Get the vertices for each endpoint
  //2. Find the x, y, and z disances of the first endpoint from the second endpoint
  //3. Find the y difference of the first endpoint from the x-z clipping plane
  //4. Find the ration of the y distance between the two endpoints and the y distances from the first endpoint and the
  //   x-z clipping plane
  //5. Take the vector between the two endpoints and multiply it by this ratio to get the exact clipping point on the
  //   x-z plane.
  //6. Save the vertex created to the slice
  vector<VertexAttributes>sliceData;
  vector<std::pair<int, int>>::iterator sliceIter = slice.begin();
  while (sliceIter != slice.end())
  {
    VertexAttributes v1 = vertexData[(*sliceIter).first];
    VertexAttributes v2 = vertexData[(*sliceIter).second];
    double xDif = v1.x - v2.x;
    double yDif = v1.y - v2.y;
    double zDif = v1.z - v2.z;
    double yDif2 = v1.y - currentYValue;
    //This is the ratio mentioned above
    double ratio = yDif2 / yDif;
    VertexAttributes v3;
    v3.x = v1.x - (xDif * ratio);
    //Be sure not to use a derived value here. Precision errors will result
    v3.y = currentYValue;
    v3.z = v1.z - (zDif * ratio);
    v3.w = 1;
    sliceData.push_back(v3);
    sliceIter++;
  }
  return sliceData;
}

void Mesh::cleanGraph(map<int, map<int, set<int>>>& savedGraph)
{
  //Remove bits from the graph that will no longer be needed
  //This speeds up the creation of slices as the algorithm progresses
  //1. Iterate through the outer map in the graph
  //2. Iterate through each inner map in the graph
  //3. Check all of the endpoints in the graph. If both endpoints lie below the x-z clipping plane, then they cannot
  //   possibly be used in another slice.
  //4. Delete the connecting vertex in the inner map.
  //5. If, at the end of the inner map ieration, the outermap's key value has
  //6. No remaining connecting vertices, then delete the outer map key index.
  map<int, map<int, set<int>>>::iterator mainGraphIterator = savedGraph.begin();
  while (mainGraphIterator != savedGraph.end())
  {
    //We have the first vertex in the edge
    double firstVertexYVal = vertexData[(*mainGraphIterator).first].y;
    map<int, set<int>>::iterator innerMapIterator = (*mainGraphIterator).second.begin();
    while (innerMapIterator != (*mainGraphIterator).second.end())
    {
      //We have the second vertex in the edge. Now find if they both are below
      //the y clipping plane
      double secondVertexYVal = vertexData[(*innerMapIterator).first].y;
      if (firstVertexYVal < currentYValue && secondVertexYVal < currentYValue)
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
  //Compute the stability of each point (irrespective of the stability of the points below it
  //1. Initialize the lowest slice to have a stability value of 0.
  //2. Go through each sice, and each point in each slice, and initialize the stabilities for each vertex to 0.
  //3. Create a number of threads equal to the maximum number of concurrently running threads on the hardware
  //4. Divide the vector of slices among the threads, assigning a section of the model for that thread to find the
  //   stability for.
  //5. Call the thread function and join the threads when they complete
  bestStabilityModel.clear();
  unsigned int i;
  vector<PointStability>firstSlice;
  PointStability p;
  p.stabilityValue = 0.0;
  p.containingSliceIndex = 0;
  p.supportingVertex1 = -1;
  p.supportingVertex2 = -1;
  for (i = 0; i < modelCrossSectionData[0].size(); i++)
  {
    firstSlice.push_back(p);
  }
  bestStabilityModel.push_back(firstSlice);
  for (i = 1; i < modelCrossSectionData.size(); i++)
  {
    vector<PointStability>stabilitySlice;
    for (unsigned int j = 0; j < modelCrossSectionData[i].size(); j++)
    {
      PointStability q;
      q.stabilityValue = 0.0;
      q.containingSliceIndex = -1;
      q.supportingVertex1 = -1;
      q.supportingVertex2 = -1;
      stabilitySlice.push_back(q);
    }
    bestStabilityModel.push_back(stabilitySlice);
  }
  //Number of threads is equal to the number of logical processors on the running hardware
  static unsigned int maxThreads = thread::hardware_concurrency();
  cout << "Running stability computation on " << maxThreads << " threads" << endl;
  if (maxThreads < 1)
  {
    maxThreads = 1;
  }
  vector<thread> t;
  for (i = 0; i < maxThreads; i++)
  {
    t.push_back(thread());
  }
  unsigned int numPerThread = modelCrossSectionData.size() / maxThreads;
  unsigned int start;
  unsigned int end = 0;
  progressCount.clear();
  for (i = 0; i < maxThreads - 1; i++)
  {
    start = (numPerThread * i);
    end = start + numPerThread;
    progressCount.push_back(0);
    t[i] = thread(&Mesh::stabilityThread, this, start, end, i);
  }
  t[maxThreads - 1] = thread(&Mesh::stabilityThread, this, end, modelCrossSectionData.size(), maxThreads - 1);
  progressCount.push_back(0);
  thread progressTracker = thread(&Mesh::progressThread, this);
  for (i = 0; i < maxThreads; i++)
  {
    t[i].join();
  }
  progressTracker.join();
}

void Mesh::stabilityThread(unsigned int start, unsigned int end, unsigned int threadID)
{
  //For all of the slices that have been assigned to this thread, compute the stability of each vertex in the slices
  //ignoring whether the vertices below them are also supported
  //1. For each slice, find the y value of the slices that are directly below slice. There may be more than one slice
  //   like this.
  //2. Get all of the slices with this y value
  //3. For each point on THIS slice, find the slice that best supports it and record the support value
  unsigned int i;
  for (i = start; i < end; i++)
  {
    double closestLowerY = findClosestLowerYValue(i);
    vector<pair<int, vector<VertexAttributes>>>lowerSlices;
    lowSlices(lowerSlices, closestLowerY);
    for (unsigned int j = 0; j < modelCrossSectionData[i].size(); j++)
    {
      bestStabilityModel[i][j] = findBestStability(modelCrossSectionData[i][j], lowerSlices);
    }
    progressCount[threadID]++;
  }
}

void Mesh::progressThread()
{
  //This is a function called in a thread that helps keep track of the process of computing stability (which can take a
  //while). It updates every second and counts how many slices have been computed
  int sliceCount = 0;
  while (sliceCount < modelCrossSectionData.size())
  {
    sliceCount = 0;
    for (unsigned int i = 0; i < progressCount.size(); i++)
    {
      sliceCount += progressCount[i];
    }
    cout << sliceCount << " out of " << modelCrossSectionData.size() << " slices completed";
    cout << '\r';
    std::chrono::milliseconds dura(1000);
    std::this_thread::sleep_for(dura);
  }
  cout << endl;
}

double Mesh::findClosestLowerYValue(int sliceIndex)
{
  //Find the y value of the slice that is directly below the slice specified by sliceIndex
  //1. Initialize the lower y value to be equal to the y value in this slice
  //2. Go through each of the lower slices and check the y values of the slices
  //3. If the y value of that sliceis greater than the current best y value but less than the y value of the slice
  //   specified by sliceIndex, then update the best slice
  double closestLowerY = modelCrossSectionData[sliceIndex][0].y;
  for (unsigned int j = 0; j < modelCrossSectionData.size(); j++)
  {
    if (closestLowerY == modelCrossSectionData[sliceIndex][0].y)
    {
      if (modelCrossSectionData[j][0].y < modelCrossSectionData[sliceIndex][0].y)
      {
        closestLowerY = modelCrossSectionData[j][0].y;
      }
    }
    else
    {
      if (modelCrossSectionData[sliceIndex][0].y > modelCrossSectionData[j][0].y)
      {
        double dif1 = modelCrossSectionData[sliceIndex][0].y - modelCrossSectionData[j][0].y;
        double dif2 = modelCrossSectionData[sliceIndex][0].y - closestLowerY;
        if (dif1 < dif2)
        {
          closestLowerY = modelCrossSectionData[j][0].y;
        }
      }
    }
  }
  return closestLowerY;
}

void Mesh::lowSlices(vector<pair<int, vector<VertexAttributes>>>& lowSlices, double lowerYValue)
{
  //Retreve a list of all of the slices equal to a specified lower value. These are all of the slice that are at a y
  //value directly below a given slice
  for (unsigned int j = 0; j < modelCrossSectionData.size(); j++)
  {
    if (modelCrossSectionData[j][0].y == lowerYValue)
    {
      lowSlices.push_back(make_pair(j, modelCrossSectionData[j]));
    }
  }
}

Mesh::PointStability Mesh::findBestStability(VertexAttributes& vertex, vector<pair<int,
                                             vector<VertexAttributes>>>& lowerSlices)
{
  //Given a vertex and a list of lower slices, find the slice that best supports the vertex and return the stability
  //1. Iterate through each of the lower slices.
  //2. Find all of the edges in the slice and link them in a perimeter.
  //3. Call clipPoints and countNumToSide to find if the vertex is directly over
  //   a slice or lies outside its bounds when projected onto the slice
  //   a. If countNumToSide return an odd value, the vertex lies inside the slice below it. Return -1 as the stability
  //   b. If countNumToSide returns an even value, then the vertex lies outside the slice below. Return a positive
  //      stability value equal to the distance
  PointStability bestStability;
  bool firstAttemptToFindBestStability = true;
  for (unsigned int i = 0; i < lowerSlices.size(); i++)
  {
    pair<int, vector<pair<VertexAttributes, VertexAttributes>>> slicePerimeter;
    fillPerimeter(slicePerimeter, lowerSlices[i].second, lowerSlices[i].first);
    vector<double>clippingPoints;
    clipPoints(slicePerimeter, clippingPoints, vertex.z);
    int count = countNumToSide(clippingPoints, vertex.x);
    PointStability distance;
    if (count % 2 != 0)
    {
      distance = findDistance(vertex, slicePerimeter);
      distance.stabilityValue = -1.0;
    }
    else
    {
      distance = findDistance(vertex, slicePerimeter);
    }
    if (firstAttemptToFindBestStability)
    {
      bestStability = distance;
      firstAttemptToFindBestStability = false;
    }
    else
    {
      if (bestStability.stabilityValue > distance.stabilityValue)
      {
        bestStability = distance;
      }
    }
  }
  return bestStability;
}

void Mesh::fillPerimeter(pair<int, vector<pair<VertexAttributes, VertexAttributes>>>& perimeter,
                         vector<VertexAttributes>& lowerSlice, int supportingSliceIndex)
{
  //Given a slice and the index of the slice that supports it, retrieve all of the vertices in the slice and link them
  //together to form edges. Also, store in the perimeter the index of the slice that this perimeter represents
  for (unsigned int k = 0; k < lowerSlice.size() - 1; k++)
  {
    perimeter.second.push_back(make_pair(lowerSlice[k], lowerSlice[k + 1]));
  }
  pair<VertexAttributes, VertexAttributes>finalEdge;
  finalEdge = make_pair(lowerSlice[lowerSlice.size() - 1], lowerSlice[0]);
  perimeter.second.push_back(finalEdge);
  perimeter.first = supportingSliceIndex;
}

void Mesh::clipPoints(pair<int, vector<pair<VertexAttributes, VertexAttributes>>>& perimeter,
                      vector<double>& clipPoints, double z)
{
  //This method is used to find if a point likes inside or outside a polygon (and therefore if a vertex in the model is
  //hanging over the edge or is supported by the bit below it.
  //1. Go through each of the edges in the perimeter. Find the z values of each endpoint in each edge.
  //2. If the two z values are on opposite sides of the vertex we are checking, find the ratio of distances between the
  //   endpoints vs. the distance between the second enpoint and the vertex (with respect to the z axis)
  //3. Apply this ratio to the difference in x values between the two endpoints to get an x value that represents a
  //   point on the line z=vertex.z where an edge on the slice clips that line. What you are effectively doing is
  //   finding all of the edges in the slice that are on either side of the vertex (left or right) and storing their
  //   positions with respect to the x axis.
  //4. Once this is done, sort these x values from smallest to largest
  for (unsigned int k = 0; k < perimeter.second.size(); k++)
  {
    double firstZ = perimeter.second[k].first.z;
    double secondZ = perimeter.second[k].second.z;
    if ((firstZ >= z && secondZ < z) || (firstZ < z && secondZ >= z))
    {
      double xDif = perimeter.second[k].first.x - perimeter.second[k].second.x;
      double zDif = perimeter.second[k].first.z - perimeter.second[k].second.z;
      double zDif2 = perimeter.second[k].first.z - z;
      double ratio = zDif2 / zDif;
      clipPoints.push_back(perimeter.second[k].first.x - (xDif * ratio));
    }
  }
  sort(clipPoints.begin(), clipPoints.end());
}

int Mesh::countNumToSide(vector<double>& clippingPoints, double x)
{
  //Given all of the points on the line z=vertex.z, find how many of them are on one side of the vertex. (clip.x < v.x).
  //Return this number
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

Mesh::PointStability Mesh::findDistance(VertexAttributes& vertex,
                                        pair<int, vector<pair<VertexAttributes, VertexAttributes>>>& perimeter)
{
  //Find how far inside or outside a polygon a given point lies. The result is always a positive number. The vertex's
  //closest distance will either have it meet up with an edge or a vertex in the slice. Start with the edges. A vertex
  //will be closest to an edge if there exists an edge on the slice which can be projected out perpendicular to its
  //slope and have it clip the vertex within the bounds of that edge. This is what we will test first.
  //1. Start by initializing a value which will remember exactly which edge the vertex was closest to. We will
  //   initialize it to slice index -1 and to vertices -1 and -1 in that slice.
  //2. Go though each edge in the slice we are comparing to.
  //3. Check if the vertex likes ON the edge
  //   a. Find the slope of this edge (xDirEdge and zDirEdge) as well as the slope of the line that is perpendicular to
  //      this edge (xDirPerp, zDirPerp)
  //   b. Also find the slope between the edge's first end point and the vertex itself.
  //   c. If these are the same, then check to see if the vertex slies ON the edge If it does, set the distance to 0 and
  //      record the edge that supports this vertex.
  //   d. If the slopes were not the same, start trying to see if the edge can be projected out onto the point
  //4. Find the length of the edge (mag). Use it to normalize the edge (newXEdge, newZEdge). Do the same for the
  //   perpendicular vectors(newXP, newZP).
  //5. For each endpoint in the edge, computing the following two values (a1,a2)
  //   {
  //     ("X component of normalized edge slope" * "Z component of ray from vertex to endpoint")
  //                                             +
  //     ("Z component of normalized edge slope" * "X component of ray from vertex to endpoint")
  //   }
  //                        /
  //   {
  //     ("X component of normalized perpendicular slope" * "X component of normalized edge slope")
  //                                                      -
  //     ("Z component of normalized perpendicular slope" * "Z component of normalized edge slope")
  //   }
  //6. Also compute the following value for each endpoint (b1,b2)
  //   {
  //     "X component of ray from vertex to endpoint" +
  //     {
  //       "X component of normalized perpendicular slope * The a value computed above
  //     }
  //   }
  //                        /
  //   "X component of of normalized edge slope"
  //
  //7. For both end points, record if both the a value and b value were both greater than or equal to 0 OR both less
  //   than 0 (but not one or the other)(intersect1 and intersect2)
  //8. If both intersect1 and intersect two are of equal value (both true or both false) then return to step 2
  //9. Otherwise, perform the calculation to find the straightline distance along a line perpendicular to the edge to
  //   the vertex. Record this distance and the edge it involves.
  //10.Throughout this process, keep track of the smallest distance computed
  //11.Find the distance from the vertex to the closest corner on the slice by by going through each vertex on the slice
  //   and finding its straighline distance to the vertex in question. Record the smallest distance found
  //12.Return the smaller of the two pointStabilities (either from an edge or from a corner)
  unsigned int k;
  double minEdgeDistance, edgeDistance, xPt, zPt, xDif, zDif;
  bool foundAnEdgeDistance = false;
  pair<int, pair<int, int>>lowerEdgeVerts = make_pair(-1, make_pair(-1, -1));
  for (k = 0; k < perimeter.second.size(); k++)
  {
    VertexAttributes v1 = perimeter.second[k].first;
    VertexAttributes v2 = perimeter.second[k].second;
    double xDirEdge = v1.x - v2.x;
    double zDirEdge = v1.z - v2.z;
    double xDirPerp = -zDirEdge;
    double zDirPerp = xDirEdge;
    double edgeSlope = xDirEdge / zDirEdge;
    double slopeBetweenEdgeFirstAndPt = (v1.x - vertex.x) / (v1.z - vertex.z);
    if (slopeBetweenEdgeFirstAndPt == edgeSlope)
    {
      if ((v1.x - vertex.x <= xDirEdge) && (v1.z - vertex.z <= zDirEdge))
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
      double a1 = (newXEdge * (v1.z - vertex.z) + newZEdge * (vertex.x - v1.x)) / den;
      double b1 = ((v1.x - vertex.x) + (newXP * a1)) / newXEdge;
      double a2 = (newXEdge * (v2.z - vertex.z) + newZEdge * (vertex.x - v2.x)) / den;
      double b2 = ((v2.x - vertex.x) + (newXP * a2)) / newXEdge;
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
            num = (v2.z * -newXP - newZP * vertex.x - vertex.z * -newXP + newZP * v2.x);
            den = ((-newXEdge * -newZP) + newZEdge * -newXP);
            double c = num / den;
            xPt = v2.x - newXEdge * c;
            zPt = v2.z - newZEdge * c;
            xDif = vertex.x - xPt;
            zDif = vertex.z - zPt;
            edgeDistance = sqrt(xDif * xDif + zDif * zDif);
            if (k != perimeter.second.size() - 1)
            {
              lowerEdgeVerts = make_pair(perimeter.first, make_pair(k, k + 1));
            }
            else
            {
              lowerEdgeVerts = make_pair(perimeter.first, make_pair(k, 0));
            }
          }
          else
          {
            num = (v2.z * -newXP + newZP * vertex.x - vertex.z * newXP - newZP * v2.x);
            den = ((-newXEdge * newZP) + newZEdge * newXP);
            double c = num / den;
            xPt = v2.x - newXEdge * c;
            zPt = v2.z - newZEdge * c;
            xDif = vertex.x - xPt;
            zDif = vertex.z - zPt;
            edgeDistance = sqrt(xDif * xDif + zDif * zDif);
            if (k != perimeter.second.size() - 1)
            {
              lowerEdgeVerts = make_pair(perimeter.first, make_pair(k, k + 1));
            }
            else
            {
              lowerEdgeVerts = make_pair(perimeter.first, make_pair(k, 0));
            }
          }
        }
        else
        {
          if (a2 >= 0)
          {
            num = (v1.z * -newXP - newZP * vertex.x - vertex.z * -newXP + newZP * v1.x);
            den = ((newXEdge * -newZP) - newZEdge * -newXP);
            double c = num / den;
            xPt = v1.x + newXEdge * c;
            zPt = v1.z + newZEdge * c;
            xDif = vertex.x - xPt;
            zDif = vertex.z - zPt;
            edgeDistance = sqrt(xDif * xDif + zDif * zDif);
            if (k != perimeter.second.size() - 1)
            {
              lowerEdgeVerts = make_pair(perimeter.first, make_pair(k, k + 1));
            }
            else
            {
              lowerEdgeVerts = make_pair(perimeter.first, make_pair(k, 0));
            }
          }
          else
          {
            num = (v1.z * -newXP + newZP * vertex.x - vertex.z * newXP - newZP * v1.x);
            den = ((newXEdge * newZP) - newZEdge * newXP);
            double c = num / den;
            xPt = v1.x + newXEdge * c;
            zPt = v1.z + newZEdge * c;
            xDif = vertex.x - xPt;
            zDif = vertex.z - zPt;
            edgeDistance = sqrt(xDif * xDif + zDif * zDif);
            if (k != perimeter.second.size() - 1)
            {
              lowerEdgeVerts = make_pair(perimeter.first, make_pair(k, k + 1));
            }
            else
            {
              lowerEdgeVerts = make_pair(perimeter.first, make_pair(k, 0));
            }
          }
        }
      }
    }
    if (!foundAnEdgeDistance)
    {
      minEdgeDistance = edgeDistance;
      foundAnEdgeDistance = true;
    }
    else
    {
      if (minEdgeDistance > edgeDistance)
      {
        minEdgeDistance = edgeDistance;
      }
    }
  }
  xDif = perimeter.second[0].first.x - vertex.x;
  zDif = perimeter.second[0].first.z - vertex.z;
  double pointDistance = sqrt(xDif * xDif + zDif * zDif);
  double dif;
  pair<int, pair<int, int>>lowerPointVerts = make_pair(perimeter.first, make_pair(0, 0));
  for (k = 0; k < perimeter.second.size(); k++)
  {
    xDif = perimeter.second[k].first.x - vertex.x;
    zDif = perimeter.second[k].first.z - vertex.z;
    dif = sqrt(xDif * xDif + zDif * zDif);
    if (dif < pointDistance)
    {
      pointDistance = dif;
      lowerPointVerts = make_pair(perimeter.first, make_pair(k, k));
    }
  }
  PointStability p;
  p.vertexCoordinates = vertex;
  if (!foundAnEdgeDistance)
  {
    p.stabilityValue = pointDistance;
    p.containingSliceIndex = lowerPointVerts.first;
    p.supportingVertex1 = lowerPointVerts.second.first;
    p.supportingVertex2 = lowerPointVerts.second.second;
    return p;
  }
  else
  {
    if (edgeDistance < pointDistance)
    {
      p.stabilityValue = edgeDistance;
      p.containingSliceIndex = lowerEdgeVerts.first;
      p.supportingVertex1 = lowerEdgeVerts.second.first;
      p.supportingVertex2 = lowerEdgeVerts.second.second;
    }
    else
    {
      p.stabilityValue = pointDistance;
      p.containingSliceIndex = lowerPointVerts.first;
      p.supportingVertex1 = lowerPointVerts.second.first;
      p.supportingVertex2 = lowerPointVerts.second.second;
    }
    return p;
  }
}

void Mesh::adjustStability()
{
  //Adjust the raw stability computed in stability thread to consider whether a particular vertex is supported by
  //vertices that are also stable. If a vertex is not stable, then not of the vertices that it supports can be
  //considered stabled either. Go through each vertex. Find the stability of a the two supporting vertices for each
  //vertex. If they are less than the stability of the supported vertex, then the stability of the supported vertex is
  //equal to the smallest of the stabilities of the two supporting vertices
  unsigned int i, j;
  for (i = 1; i < bestStabilityModel.size(); i++)
  {
    for (j = 0; j < bestStabilityModel[i].size(); j++)
    {
      PointStability pStability = bestStabilityModel[i][j];
      PointStability lowerStability1 =
          bestStabilityModel[pStability.containingSliceIndex][pStability.supportingVertex1];
      PointStability lowerStability2 =
          bestStabilityModel[pStability.containingSliceIndex][pStability.supportingVertex2];
      if (lowerStability1.stabilityValue >= lowerStability2.stabilityValue)
      {
        if (lowerStability1.stabilityValue >= pStability.stabilityValue)
        {
          pStability.stabilityValue = lowerStability1.stabilityValue;
          bestStabilityModel[i][j] = pStability;
        }
        else
        {
          bestStabilityModel[i][j] = pStability;
        }
      }
      else if (lowerStability2.stabilityValue >= lowerStability1.stabilityValue)
      {
        if (lowerStability2.stabilityValue >= pStability.stabilityValue)
        {
          pStability.stabilityValue = lowerStability2.stabilityValue;
          bestStabilityModel[i][j] = lowerStability2;
        }
        else
        {
          bestStabilityModel[i][j] = pStability;
        }
      }
    }
  }
  splitIntoSubModels();
}

void Mesh::splitIntoSubModels()
{
  //Begin by pairing down stability model to just the vertices that were in the original vertex data. This makes it
  //possible for us to consider the original graph in terms of its stability rather than y value
  pairedDownModel.clear();
  unsigned int i, j, k;
  for (i = 0; i < vertexData.size(); i++)
  {
    for (j = 0; j < bestStabilityModel.size(); j++)
    {
      for (k = 0; k < bestStabilityModel[j].size(); k++)
      {
        if (bestStabilityModel[j][k].vertexCoordinates.x == vertexData[i].x &&
            bestStabilityModel[j][k].vertexCoordinates.y == vertexData[i].y &&
            bestStabilityModel[j][k].vertexCoordinates.z == vertexData[i].z)
        {
          pairedDownModel.push_back(bestStabilityModel[j][k]);
        }
      }
    }
  }
  stabilityModelsUnderConsideration.push_back(pairedDownModel);
}

void Mesh::findBestOrientation(double tolerance)
{
  bestStabilityModel.clear();
  //Considering all 4 stability models, find the most supported vertices given the provided tolerance
  int i;
  unsigned int j, k;
  double stability[4];
  for (i = 0; i < 4; i++)
  {
    int greenCount = 0;
    int totalNumberOfVerts = 0;
    for (j = 0; j < stabilityModelsUnderConsideration[i].size(); j++)
    {
      if (stabilityModelsUnderConsideration[i][j].stabilityValue <= tolerance)
      {
        greenCount++;
      }
      totalNumberOfVerts++;
    }
    stability[i] = (double)greenCount / (double)totalNumberOfVerts;
  }
  int bestIndex = 0;
  double bestStability = stability[0];
  for (i = 1; i < 4; i++)
  {
    if (stability[i] > bestStability)
    {
      bestStability = stability[i];
      bestIndex = i;
    }
  }
  pairedDownModel = stabilityModelsUnderConsideration[bestIndex];
}

Mesh::~Mesh(void)
{
  //Nothing to do
}
void Mesh::draw(GLint objectColorLocation, double tolerance)
{
  //Given the tolerance provided, find the model that has the most stable vertices. Draw that model.
  findBestOrientation(tolerance);
  for (unsigned int i = 0; i < pairedDownModel.size(); i++)
  {
    glm::vec3 color;
    if (pairedDownModel[i].stabilityValue > tolerance)
    {
      color = glm::vec3(1, 0, 0);
    }
    else
    {
      color = glm::vec3(0, 1, 0);
    }
    glVertexAttrib3fv(objectColorLocation, glm::value_ptr(color));
    glBegin(GL_POINTS);
    VertexAttributes v = pairedDownModel[i].vertexCoordinates;
    glVertex3d(v.x, v.y, v.z);
    glEnd();
  }
}
void Mesh::drawSlice(unsigned int sliceIndex, GLint objectColorLocation, double tolerance)
{
  //Draw each vertex in each slice. Red = unstable. Green = stable
  for (unsigned j = 0; j < bestStabilityModel[sliceIndex].size(); j++)
  {
    glm::vec3 color;
    if (bestStabilityModel[sliceIndex][j].stabilityValue > tolerance)
    {
      color = glm::vec3(1, 0, 0);
    }
    else
    {
      color = glm::vec3(0, 1, 0);
    }
    glVertexAttrib3fv(objectColorLocation, glm::value_ptr(color));
    glBegin(GL_POINTS);
    VertexAttributes v = bestStabilityModel[sliceIndex][j].vertexCoordinates;
    glVertex3d(v.x, v.y, v.z);
    glEnd();
  }
}
