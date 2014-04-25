#include "View.h"
#include <GL/glew.h>
#include <cstdlib>
#include <fstream>
#include <vector>
using namespace std;
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Plane.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Box.h"
#include "ObjectXMLReader.h"
#include "Cone.h"

View::View()
{
}

View::~View()
{
    for (unsigned int i=0;i<objectsList.size();i++)
    {
        delete objectsList[i];
    }
    objectsList.clear();
}

void View::resize(int w, int h)
{
    WINDOW_WIDTH = w;
    WINDOW_HEIGHT = h;
    proj = glm::perspective(60.0f,(float)WINDOW_WIDTH/WINDOW_HEIGHT,0.1f,10000.0f);
}

void View::openFile(string filename)
{
    for (unsigned int i=0;i<objectsList.size();i++)
    {
        delete objectsList[i];
    }
    objectsList.clear();
    objectsList = ObjectXMLReader::readObjectXMLFile(filename);
}

void View::initialize()
{
    ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER,"trianglesV2.vert"},
        {GL_FRAGMENT_SHADER,"triangles.frag"},
        {GL_NONE,""}
    };
    GLuint program = createShaders(shaders);
    glUseProgram(program);
    projectionLocation = glGetUniformLocation(program,"projection");
    modelviewLocation = glGetUniformLocation(program,"modelview");
    objectColorLocation = glGetAttribLocation(program,"vColor");

    //MODIFY THESE LINES TO CHANGE THE INPUT FILE
    //objectsList = ObjectXMLReader::readObjectXMLFile("face-no-cap.txt");
    //objectsList = ObjectXMLReader::readObjectXMLFile("face.txt");
    //objectsList = ObjectXMLReader::readObjectXMLFile("locomotive.txt");
    objectsList = ObjectXMLReader::readObjectXMLFile("objfile.txt");
}

void View::draw(float Xangle, float Yangle)
{
    //glEnable (GL_POLYGON_SMOOTH);
    //glEnable (GL_LINE_SMOOTH);
    //glEnable (GL_BLEND);
    //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glHint (GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);
    //set the perspective to start in front of the object 200 pixels out and 50 pixels up and look at the origin
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    modelview = glm::lookAt(glm::vec3(50,50,50),glm::vec3(0,0,0),glm::vec3(0,1,0));
    glUniformMatrix4fv(projectionLocation,1,GL_FALSE,glm::value_ptr(proj));
    //for each shape
    for (unsigned int i=0;i<objectsList.size();i++)
    {
        //retrieve the previous transformation
        glm::mat4 transform = objectsList[i]->getTransform();
        //retrieve the color
        glm::vec3 color = objectsList[i]->getColor();
        //apply the y rotation
        transform = glm::rotate(glm::mat4(1.0), Yangle, glm::vec3(1, 0, 0)) * transform;
        //apply the x rotation
        transform = glm::rotate(glm::mat4(1.0), Xangle, glm::vec3(0, 1, 0)) * transform;
        //clear the transformation to prevent overloading memory with unnecessary saved transformations
        objectsList[i]->clearTransform();
        //save the transformation
        objectsList[i]->setTransform(transform);
        //apply the transformation
        glUniformMatrix4fv(modelviewLocation,1,GL_FALSE,glm::value_ptr(modelview * objectsList[i]->getTransform()));
        //apply the color
        glVertexAttrib3fv(objectColorLocation,glm::value_ptr(color));
        //draw
        objectsList[i]->draw();
    }
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glFinish();
}

GLuint View::createShaders(ShaderInfo *shaders)
{
    ifstream file;
    GLuint shaderProgram;
    GLint linked;
    ShaderInfo *entries = shaders;
    shaderProgram = glCreateProgram();
    while (entries->type !=GL_NONE)
    {
        file.open(entries->filename.c_str());
        GLint compiled;
        if (!file.is_open())
            return false;
        string source,line;
        getline(file,line);
        while (!file.eof())
        {
            source = source + "\n" + line;
            getline(file,line);
        }
        file.close();
        const char *codev = source.c_str();
        entries->shader = glCreateShader(entries->type);
        glShaderSource(entries->shader,1,&codev,NULL);
        glCompileShader(entries->shader);
        glGetShaderiv(entries->shader,GL_COMPILE_STATUS,&compiled);
        if (!compiled)
        {
            printShaderInfoLog(entries->shader);
            for (ShaderInfo *processed = shaders;processed->type!=GL_NONE;processed++)
            {
                glDeleteShader(processed->shader);
                processed->shader = 0;
            }
            return 0;
        }
        glAttachShader( shaderProgram, entries->shader );
        entries++;
    }
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram,GL_LINK_STATUS,&linked);
    if (!linked)
    {
        printShaderInfoLog(entries->shader);
        for (ShaderInfo *processed = shaders;processed->type!=GL_NONE;processed++)
        {
            glDeleteShader(processed->shader);
            processed->shader = 0;
        }
        return 0;
    }
    return shaderProgram;
}

void View::printShaderInfoLog(GLuint shader)
{
    int infologLen = 0;
    int charsWritten = 0;
    GLubyte *infoLog;
    glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&infologLen);
    if (infologLen>0)
    {
        infoLog = (GLubyte *)malloc(infologLen);
        if (infoLog != NULL)
        {
            glGetShaderInfoLog(shader,infologLen,&charsWritten,(char *)infoLog);
            printf("InfoLog: %s\n\n",infoLog);
            free(infoLog);
        }
    }
}

void View::getOpenGLVersion(int *major,int *minor)
{
    const char *verstr = (const char *)glGetString(GL_VERSION);
    if ((verstr == NULL) || (sscanf_s(verstr,"%d.%d",major,minor)!=2))
    {
        *major = *minor = 0;
    }
}

void View::getGLSLVersion(int *major,int *minor)
{
    int gl_major,gl_minor;
    getOpenGLVersion(&gl_major,&gl_minor);
    *major = *minor = 0;
    if (gl_major==1)
    {
        const char *extstr = (const char *)glGetString(GL_EXTENSIONS);
        if ((extstr!=NULL) && (strstr(extstr,"GL_ARB_shading_language_100")!=NULL))
        {
            *major = 1;
            *minor = 0;
        }
    }
    else if (gl_major>=2)
    {
        const char *verstr = (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
        if ((verstr==NULL) || (sscanf_s(verstr,"%d.%d",major,minor) !=2))
        {
            *major = 0;
            *minor = 0;
        }
    }
}
