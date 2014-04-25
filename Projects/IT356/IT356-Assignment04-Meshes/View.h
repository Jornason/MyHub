#ifndef VIEW_H
#define VIEW_H
#include <cstdlib>
#include <fstream>
#include "ObjectXMLReader.h"
using namespace std;

/*
 * View class
 * This class defines a view for the program
 * It contains a scenegraph, window parameters, a projection matrix, and a modelview matrix
 */

class View
{
    #define BUFFER_OFFSET(offset) ((void *)(offset))
    //Shader info
    typedef struct {
        GLenum       type;
        string  filename;
        GLuint       shader;
    } ShaderInfo;


public:
    //Default constructor
    View();
    //DESTRUCTOR
    ~View();
    //Resize the window
    void resize(int w,int h);
    //Initialize everything
    void initialize();
    //draw the scene
    void draw();
    //void draw(float deltaXAngle, float deltaYAngle);
    //get the openGL version
    void getOpenGLVersion(int *major,int *minor);
    //get the GLSL Version
    void getGLSLVersion(int *major,int *minor);
    //Animate the scene
    void animate();
    //SETTERS AND GETTERS
    Scenegraph* getScenegraph();
    void setScenegraph(Scenegraph* scene);
    vector<Light> listofLights;
    map<string, GLuint> textureMap;
    stack<glm::mat4> transformationStack;
protected:
    //Create the shaders
    GLuint createShaders(ShaderInfo *shaders);
    GLuint program;
    //diagnostic information
    void printShaderInfoLog(GLuint shader);
private:
    //The dimensions of the window
    int WINDOW_WIDTH,WINDOW_HEIGHT;
    //Location info for the shader
    //GLint projectionLocation,modelviewLocation ,objectColorLocation;
    GLint projectionLocation,modelViewLocation,normalMatrixLocation;
    GLint numLightsLocation;
    GLint mat_ambientLocation,mat_diffuseLocation,mat_specularLocation,mat_shininessLocation;
    GLint light_ambientLocation,light_diffuseLocation,light_specularLocation,light_positionLocation;
    GLint textureLocation, textureMatrixLocation;
    //The scene
    Scenegraph *scene;
    //The projection matrix
    glm::mat4 proj;
    //The modelview matrix
    glm::mat4 modelview;
};

#endif
