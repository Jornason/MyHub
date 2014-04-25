#include "View.h"

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
    ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER,"phong-multiple.vert"},
        {GL_FRAGMENT_SHADER,"phong-multiple.frag"},
        {GL_NONE,""}
    };
    program = createShaders(shaders);
    glUseProgram(program);
    //get the locations
    projectionLocation = glGetUniformLocation(program,"projection");
    modelViewLocation = glGetUniformLocation(program,"modelview");
    normalMatrixLocation = glGetUniformLocation(program,"normalmatrix");
    numLightsLocation = glGetUniformLocation(program,"numLights");
    textureMatrixLocation = glGetUniformLocation(program,"texturematrix");

    mat_ambientLocation = glGetUniformLocation(program,"material.ambient");
    mat_diffuseLocation = glGetUniformLocation(program,"material.diffuse");
    mat_specularLocation = glGetUniformLocation(program,"material.specular");
    mat_shininessLocation = glGetUniformLocation(program,"material.shininess");
    textureLocation = glGetUniformLocation(program,"image");

    //objectColorLocation = glGetAttribLocation(program,"vColor");
    //set the perspective of the camera
    modelview = glm::lookAt(glm::vec3(400,400,400),glm::vec3(0,0,0),glm::vec3(0,1,0));



    //MODIFY THESE LINES TO CHANGE THE INPUT FILE
    //scene = ObjectXMLReader::readObjectXMLFile("face-no-cap.txt", objectColorLocation, modelviewLocation, modelview);
    //scene = ObjectXMLReader::readObjectXMLFile("face.txt", objectColorLocation, modelviewLocation, modelview);
    //scene = ObjectXMLReader::readObjectXMLFile("locomotive.txt", objectColorLocation, modelViewLocation, modelview);
    //scene = ObjectXMLReader::readObjectXMLFile("locomotive.txt", modelViewLocation, modelview);
    //scene = ObjectXMLReader::readObjectXMLFile("locomotive.txt", modelViewLocation, modelview, normalMatrixLocation, mat_ambientLocation, mat_diffuseLocation, mat_specularLocation, mat_shininessLocation, textureMap, textureLocation, textureMatrixLocation);
    scene = ObjectXMLReader::readObjectXMLFile("mesh.xml", modelViewLocation, modelview, normalMatrixLocation, mat_ambientLocation, mat_diffuseLocation, mat_specularLocation, mat_shininessLocation, textureMap, textureLocation, textureMatrixLocation);
    //scene = ObjectXMLReader::readObjectXMLFile("face-hierarchy.txt", objectColorLocation, modelviewLocation, modelview);
    //scene = ObjectXMLReader::readObjectXMLFile("simple.txt", objectColorLocation, modelviewLocation, modelview);

    glm::mat4 transform = scene->getRoot()->getTransform();
    //glm::vec4 transform = glm::vec4(0.0);


    transformationStack.push(transform);
    transformationStack.push(transformationStack.top());

    scene->lightList = scene->getLights(transformationStack);
    //Feed the lights to the shader

    scene->enableLights(scene->lightList, program);
    scene->lightList.clear();




}

void View::draw()
//void View::draw(float Xangle, float Yangle)
{
    //Tell the shader where the projection location is
    glUniformMatrix4fv(projectionLocation,1,GL_FALSE,glm::value_ptr(proj));
    glUniform1i(numLightsLocation,scene->lightlocation.size());
    //light properties

    //Get the root transformation
    //cout<<"a"<<endl;
    glm::mat4 transform = scene->getRoot()->getTransform();
    //cout<<"b"<<endl;
    //clear the transformation matrix
    scene->getRoot()->clearTransform();
    //transform = glm::rotate(glm::mat4(1.0), Yangle, glm::vec3(1, 0, 0)) * transform;
    //apply the x rotation
    //transform = glm::rotate(glm::mat4(1.0), Xangle, glm::vec3(0, 1, 0)) * transform;
    //cout<<"c"<<endl;
    //set the transformation of the root
    scene->getRoot()->setTransform(transform);
    //cout<<"d"<<endl;
    //push the transformation to the stack
    transformationStack.push(transform);
    //cout<<"e"<<endl;

    //cout<<"f"<<endl;

    //cout<<"g"<<endl;

    //cout<<"1"<<endl;
    scene->lightList = scene->getLights(transformationStack);
    scene->enableLights(scene->lightList, program);
    scene->lightList.clear();
    transformationStack.pop();
    //cout<<"2"<<endl;
    //transformationVecStack.pop();
    //draw the scene
    //cout<<"3"<<endl;


    //transformation.push(transform);
    //cout<<"4"<<endl;
    transformationStack.push(transform);

    scene->draw(transformationStack, program);
    //cout<<"5"<<endl;

    //pop the transformation from the stack
    transformationStack.pop();
    //cout<<"6"<<endl;

    glFlush();
    //done
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
