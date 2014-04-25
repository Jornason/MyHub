#ifndef VIEW_H
#define VIEW_H
#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <map>
#include <glm/glm.hpp>
#include "Scenegraph.h"
#include "Plane.h"
using namespace std;
class View
{
#define BUFFER_OFFSET(offset) ((void *)(offset))
    typedef struct
    {
      GLenum type;
      string filename;
      GLuint shader;
    } ShaderInfo;
    typedef enum {OPENGL, RAYTRACER} RENDERMODE;
  public:
    View();
    ~View();
    void resize(int w, int h);
    void initialize();
    void draw();
    void animate(float time);
    void openFile(string& filename);
    void getOpenGLVersion(int* major, int* minor);
    void getGLSLVersion(int* major, int* minor);
    inline void initOpenGLMode()
    {
      mode = OPENGL;
    }
    inline void initRaytraceMode()
    {
      mode = RAYTRACER;
    }
  protected:
    GLuint createShaders(ShaderInfo* shaders);
    void printShaderInfoLog(GLuint shader);
  private:
    int WINDOW_WIDTH, WINDOW_HEIGHT;
    GLint projectionLocation, modelviewLocation, objectColorLocation;
    GLint mat_ambientLocation, mat_diffuseLocation, mat_specularLocation;
    GLint mat_shininessLocation, rtProjLocation, rtModelViewLocation;
    GLint rtTextureLocation;
    GLuint shaderProgram, rtProgram, rtTextureID;
    Scenegraph* graph;
    Plane* rtPlane;
    RENDERMODE mode;
    stack<glm::mat4> proj, modelview;
    glm::mat4 worldToView;
};
#endif
