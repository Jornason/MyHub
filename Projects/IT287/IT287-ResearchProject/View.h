#ifndef VIEW_H
#define VIEW_H
#include<GL/glew.h>
#include<GL/gl.h>
#include<string>
#include<stack>
#include<glm/glm.hpp>
#include"object.h"
using namespace std;
class View
{
    #define BUFFER_OFFSET(offset)((void*)(offset))

    //Shader info allows a shader to be constructed using GLSL
    //The type of shader, its source file name, and a unique identifer are specified
    typedef struct
    {
      GLenum type;
      string filename;
      GLuint shader;
    }
    ShaderInfo;
  public:
    //Default constructor
    View();

    //Destructor
    ~View();

    //Resize the view
    void resize(int w, int h);

    //Initialize the view
    void initialize();

    //Render the scene
    void draw(float deltaXAngle, float deltaYAngle, double tol, double scale);

    //Display OpenGL and GLSL versions
    void getOpenGLVersion(int* major, int* minor);
    void getGLSLVersion(int* major, int* minor);
  public:

    //This holds all of the objects being rendered
    vector<Object*>objList;

    //The file containing the scene data
    string filename;

  protected:

    //constructor shaders based on shader source code
    GLuint createShaders(ShaderInfo* shaders);

    //Print shader debug info
    void printShaderInfoLog(GLuint shader);
  private:

    //Window dimentions
    int WIN_WIDTH, WIN_HEIGHT;

    //Unique identifiers for the projection, modelview and object color
    GLint projLoc, mvLoc, objColLoc;

    //projection and model view matrices
    glm::mat4 proj, modelview;
};
#endif
