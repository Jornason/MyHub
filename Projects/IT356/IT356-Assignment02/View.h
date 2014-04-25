#ifndef VIEW_H
#define VIEW_H
#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <stack>
using namespace std;
#include <glm/glm.hpp>
#include "Object.h"

class View
{
    #define BUFFER_OFFSET(offset) ((void *)(offset))
    typedef struct {
        GLenum       type;
        string  filename;
        GLuint       shader;
    } ShaderInfo;
public:
    View();
    ~View();
    void resize(int w,int h);
    void initialize();
    void draw(float deltaXAngle, float deltaYAngle);
    void openFile(string filename);
    void getOpenGLVersion(int *major,int *minor);
    void getGLSLVersion(int *major,int *minor);
    vector<Object *> objectsList;
protected:
    GLuint createShaders(ShaderInfo *shaders);
    void printShaderInfoLog(GLuint shader);
private:
    int WINDOW_WIDTH,WINDOW_HEIGHT;
    GLint projectionLocation,modelviewLocation,objectColorLocation;

    glm::mat4 proj;
    glm::mat4 modelview;
};

#endif
