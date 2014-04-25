#include "View.h"
// clipping planes
GLdouble eqn1[4] = { 1.0, 0.0, 0.0, 0.0 };
GLdouble eqn2[4] = { 0.0, 1.0, 0.0, 0.0 };
GLdouble eqn3[4] = { 0.0, 0.0, 1.0, 0.0 };

// translation for the clipping planes
GLfloat g_clip_x = 0.0f;
GLfloat g_clip_y = 0.0f;
GLfloat g_clip_z = 0.0f;

// modelview stuff
GLfloat g_angle_y = 32.0f;
GLfloat g_inc = 0.0f;
GLfloat g_eye_y = 0;

View::View()
{

}

View::~View()
{
    delete scene;
}

void View::resize(int w, int h)
{
    //Apply a new project to the window when it is resized
    WINDOW_WIDTH = w;
    WINDOW_HEIGHT = h;
    proj = glm::perspective(60.0f,(float)WINDOW_WIDTH/WINDOW_HEIGHT,0.1f,10000.0f);
}

void View::initialize()
{
    //Set the shader
    ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER,"trianglesV2.vert"},
        {GL_FRAGMENT_SHADER,"triangles.frag"},
        {GL_NONE,""}
    };
    GLuint program = createShaders(shaders);
    glUseProgram(program);
    //get the locations
    projectionLocation = glGetUniformLocation(program,"projection");
    modelviewLocation = glGetUniformLocation(program,"modelview");
    objectColorLocation = glGetAttribLocation(program,"vColor");
    //set the perspective of the camera
    modelview = glm::lookAt(glm::vec3(400,400,400),glm::vec3(0,0,0),glm::vec3(0,1,0));

    //MODIFY THESE LINES TO CHANGE THE INPUT FILE
    //scene = ObjectXMLReader::readObjectXMLFile("face-no-cap.txt", objectColorLocation, modelviewLocation, modelview);
    //scene = ObjectXMLReader::readObjectXMLFile("face.txt", objectColorLocation, modelviewLocation, modelview);
    scene = ObjectXMLReader::readObjectXMLFile("locomotivescenegraph.txt", objectColorLocation, modelviewLocation, modelview);
    //scene = ObjectXMLReader::readObjectXMLFile("face-hierarchy.txt", objectColorLocation, modelviewLocation, modelview);
    //scene = ObjectXMLReader::readObjectXMLFile("simple.txt", objectColorLocation, modelviewLocation, modelview);
}

void View::draw()
{
    //Tell the shader where the projection location is
    glUniformMatrix4fv(projectionLocation,1,GL_FALSE,glm::value_ptr(proj));
    //Get the root transformation
    glm::mat4 transform = scene->getRoot()->getTransform();
    //clear the transformation matrix
    scene->getRoot()->clearTransform();
    //set the transformation of the root
    scene->getRoot()->setTransform(transform);
    //push the transformation to the stack
    stack<glm::mat4> transformation;
    transformation.push(transform);
    //draw the scene
    scene->draw(transformation);

    glPopMatrix( );

    //pop the transformation from the stack
    transformation.pop();
    //done
    glFinish();
}

void View::animate()
{
    //increase the angle of the animation
    this->scene->increaseAngle();
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

Scenegraph* View::getScenegraph()
{
    return scene;
}

void View::setScenegraph(Scenegraph* scene)
{
    this->scene = scene;
}
