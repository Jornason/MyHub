#ifndef OBJECT_H_
#define OBJECT_H_
#include<GL/glew.h>
#include<GL/gl.h>
#include<vector>
#include<string>
#include<map>
#include<set>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<stack>
using namespace std;
#define BUFSET(offset)((void*)(offset))
struct VAttrs
{
  double x;
  double y;
  double z;
  double w;
};
class Object
{
  protected:
    enum Buffer_IDs
    {
      VertexBuffer, IndexBuffer, NumBuffers
    };
    enum Attrib_IDs
    {
      vPos = 0
    };
  protected:
    GLuint VAO;
    GLuint buffers[NumBuffers];
    vector<VAttrs>vertexData;
    vector<GLuint>triangleIndices;
    glm::vec3 color;
    stack<glm::mat4>transform;
    string name;
  public:
    Object(string name = "")
    {
      glGenVertexArrays(1, &VAO);
      glBindVertexArray(VAO);
      glGenBuffers(NumBuffers, buffers);
      setName(name);
    }
    virtual~Object(void)
    {
      if (VAO != 0)
      {
        glDeleteBuffers(NumBuffers, buffers);
        glDeleteVertexArrays(1, &VAO);
      }
    }
    virtual void draw(GLint objColLoc, double tolerance)
    {
      glBindVertexArray(VAO);
      unsigned int tSize = triangleIndices.size();
      glDrawElements(GL_TRIANGLES, tSize, GL_UNSIGNED_INT, BUFSET(0));
    }
    void setTransform(glm::mat4& obj)
    {
      transform.push(obj);
    }
    glm::mat4 getTransform()
    {
      return transform.top();
    }
    void clearTransform()
    {
      transform.pop();
    }
    void setName(const string& name)
    {
      this->name = name;
    }
    virtual void setColor(float r, float g, float b)
    {
      color = glm::vec3(r, g, b);
    }
    glm::vec3 getColor()
    {
      return color;
    }
    void passToGPU()
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
};
#endif
