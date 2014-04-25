#ifndef VIEW_H
#define VIEW_H
#include<GL/glew.h>
#include<GL/gl.h>
#include<string>
#include<stack>
#include<glm/glm.hpp>
#include"mesh.h"
using namespace std;
class View
{
#define BUFFER_OFFSET(offset)((void*)(offset))
    typedef struct
    {
      GLenum type;
      string filename;
      GLuint shader;
    }
    ShaderInfo;
  public:
    View();
    ~View();
    void resize(int w, int h);
    void initialize();
    void draw(float deltaXAngle, float deltaYAngle, double tol, double scale);
    void getOpenGLVersion(int* major, int* minor);
    void getGLSLVersion(int* major, int* minor);
  public:
    vector<Mesh*>meshList;
    string filename;
    string name;
  protected:
    GLuint createShaders(ShaderInfo* shaders);
    void printShaderInfoLog(GLuint shader);
  private:
    int WIN_WIDTH, WIN_HEIGHT;
    GLint projLoc, mvLoc, objColLoc;
    glm::mat4 proj, modelview;
};
#endif
