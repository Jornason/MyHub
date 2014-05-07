#ifndef OBJECT_H
#define OBJECT_H
#include<GL/glew.h>
#include<GL/gl.h>
#include<vector>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<stack>
using namespace std;
#define BUFSET(offset)((void*)(offset))

class Object
{
  public:
    //A struct to define a vertex. Contains the x, y, and, z coordinates as well as a "w" to tell us that this is a
    //vertex (always equal to 1)
    struct VertexAttributes
    {
      double x;
      double y;
      double z;
      double w;
    };

    //Default constructor
    Object();

    //Destructor
    virtual ~Object();
  protected:

    //Enumerate Buffer ID's
    //The vertex buffer holes vertex data
    //The index buffer holes the triangle face mapping data
    enum Buffer_IDs
    {
      VertexBuffer, IndexBuffer, NumBuffers
    };

    //Enumerate attributes's for the GPU to consider. We only care about position
    enum Attrib_IDs
    {
      vPos = 0
    };

    //The vertex array object for this object (how the GPU identifies it)
    GLuint VAO;

    //The object data buffers
    GLuint buffers[NumBuffers];

    //The vector containing all of the vertex coordinates
    vector<VertexAttributes>vertexData;

    //The vector containing all of the triangle face mapping data. Every 3 indices makes up a triangle. The 3 values of
    //that triangle correspond to indices in vertexData. So a triplet that is 1,4,5 means this triangle is formed by
    //vertex 1, 4, and 5 in vertexData
    vector<GLuint>triangleIndices;

    //The color of each vertex
    glm::vec3 color;

    //The transformation matrix for this object
    stack<glm::mat4>transform;
  public:

    //Setters and gettesr for transoformations
    void setTransform(glm::mat4& obj);
    glm::mat4 getTransform();

    //Pop the top of the transformation stack
    void clearTransform();

    //Setter and getter for the color
    void setColor(float r, float g, float b);
    glm::vec3 getColor();

    //Pass the object data to the GPU so it can render it
    void passToGPU();

    //The draw function
    virtual void draw(GLint objColLoc, double tolerance);
};

#endif // OBJECT_H
