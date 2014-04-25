#include"View.h"
#include<GL/glew.h>
#include<cstdlib>
#include<fstream>
#include<vector>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
using namespace std;

View::View()
{
  filename = "";
  name = "";
}

View::~View()
{
  for (unsigned int i = 0; i < meshList.size(); i++)
  {
    delete meshList[i];
  }
  meshList.clear();
}

void View::resize(int w, int h)
{
  WIN_WIDTH = w;
  WIN_HEIGHT = h;
  proj = glm::perspective(60.0f, (float)WIN_WIDTH / WIN_HEIGHT, 0.1f, 10000.0f);
}

void View::initialize()
{
  for (unsigned int i = 0; i < meshList.size(); i++)
  {
    delete meshList[i];
  }
  meshList.clear();
  ShaderInfo shaders[] =
  {
    {GL_VERTEX_SHADER, "trianglesV2.vert"},
    {GL_FRAGMENT_SHADER, "triangles.frag"},
    {GL_NONE, ""}
  };
  GLuint program = createShaders(shaders);
  glUseProgram(program);
  projLoc = glGetUniformLocation(program, "projection");
  mvLoc = glGetUniformLocation(program, "modelview");
  objColLoc = glGetAttribLocation(program, "vColor");
  Mesh* model = new Mesh(name, filename);
  model->setTransform(glm::mat4(1.0));
  meshList.push_back(model);
}

void View::draw(float Xangle, float Yangle, double tolerance, double scale)
{
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glm::vec3 pov = glm::vec3(0, 0, 75);
  modelview = glm::lookAt(pov, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
  for (unsigned int i = 0; i < meshList.size(); i++)
  {
    glm::mat4 id = glm::mat4(1.0);
    glm::mat4 transform = meshList[i]->getTransform();
    transform = glm::rotate(id, Yangle, glm::vec3(1, 0, 0)) * transform;
    transform = glm::rotate(id, Xangle, glm::vec3(0, 1, 0)) * transform;
    transform = glm::scale(id, glm::vec3(scale, scale, scale)) * transform;
    meshList[i]->clearTransform();
    meshList[i]->setTransform(transform);
    glm::mat4 objTransf = modelview * meshList[i]->getTransform();
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(objTransf));
    meshList[i]->draw(objColLoc, tolerance);
  }
  glFinish();
}

GLuint View::createShaders(ShaderInfo* shaders)
{
  ifstream file;
  GLuint shaderProgram;
  GLint linked;
  ShaderInfo* entries = shaders;
  shaderProgram = glCreateProgram();
  while (entries->type != GL_NONE)
  {
    file.open(entries->filename.c_str());
    GLint compiled;
    if (!file.is_open())
    {
      return false;
    }
    string source, line;
    getline(file, line);
    while (!file.eof())
    {
      source = source + "\n" + line;
      getline(file, line);
    }
    file.close();
    const char* codev = source.c_str();
    entries->shader = glCreateShader(entries->type);
    glShaderSource(entries->shader, 1, &codev, NULL);
    glCompileShader(entries->shader);
    glGetShaderiv(entries->shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
      printShaderInfoLog(entries->shader);
      for (ShaderInfo* prcssd = shaders; prcssd->type != GL_NONE; prcssd++)
      {
        glDeleteShader(prcssd->shader);
        prcssd->shader = 0;
      }
      return 0;
    }
    glAttachShader(shaderProgram, entries->shader);
    entries++;
  }
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);
  if (!linked)
  {
    printShaderInfoLog(entries->shader);
    for (ShaderInfo* prcssd = shaders; prcssd->type != GL_NONE; prcssd++)
    {
      glDeleteShader(prcssd->shader);
      prcssd->shader = 0;
    }
    return 0;
  }
  return shaderProgram;
}

void View::printShaderInfoLog(GLuint shader)
{
  int infologLen = 0;
  int charsWritten = 0;
  GLubyte* infoLog;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);
  if (infologLen > 0)
  {
    infoLog = (GLubyte*)malloc(infologLen);
    if (infoLog != NULL)
    {
      glGetShaderInfoLog(shader, infologLen, &charsWritten, (char*)infoLog);
      printf("InfoLog: %s\n\n", infoLog);
      free(infoLog);
    }
  }
}

void View::getOpenGLVersion(int* major, int* minor)
{
  const char* verstr = (const char*)glGetString(GL_VERSION);
  if ((verstr == NULL) || (sscanf_s(verstr, "%d.%d", major, minor) != 2))
  {
    *major = *minor = 0;
  }
}

void View::getGLSLVersion(int* major, int* minor)
{
  int gl_major, gl_minor;
  getOpenGLVersion(&gl_major, &gl_minor);
  *major = *minor = 0;
  if (gl_major == 1)
  {
    const char* estr = (const char*)glGetString(GL_EXTENSIONS);
    if ((estr != NULL) && (strstr(estr, "GL_ARB_shading_language_100") != NULL))
    {
      *major = 1;
      *minor = 0;
    }
  }
  else if (gl_major >= 2)
  {
    const char* verstr = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
    if ((verstr == NULL) || (sscanf_s(verstr, "%d.%d", major, minor) != 2))
    {
      *major = 0;
      *minor = 0;
    }
  }
}
