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
    struct VertexAttributes
    {
      double x;
      double y;
      double z;
      double w;
    };
    Object();
    virtual ~Object();
  protected:
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
    vector<VertexAttributes>vertexData;
    vector<GLuint>triangleIndices;
    glm::vec3 color;
    stack<glm::mat4>transform;
  public:
    void setTransform(glm::mat4& obj);
    glm::mat4 getTransform();
    void clearTransform();
    void setColor(float r, float g, float b);
    glm::vec3 getColor();
    void passToGPU();
    virtual void draw(GLint objColLoc, double tolerance);
};

#endif // OBJECT_H
