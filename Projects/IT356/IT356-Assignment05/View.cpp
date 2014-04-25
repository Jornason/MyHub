#include "View.h"
#include <GL/glew.h>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Plane.h"
#include "ObjectXMLReader.h"
using namespace std;
View::View()
{
  graph = NULL;
  shaderProgram = 0;
  proj.push(glm::mat4(1.0));
  modelview.push(glm::mat4(1.0));
  mode = OPENGL;
  rtTextureID = -1;
}
View::~View()
{
  if (graph != NULL)
    delete graph;
  if (rtPlane != NULL)
    delete rtPlane;
}
void View::resize(int w, int h)
{
  WINDOW_WIDTH = w;
  WINDOW_HEIGHT = h;
  proj.top() = glm::perspective(60.0f, (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 10000.0f);
  rtPlane->setTransform(glm::rotate(glm::mat4(1.0), 90.0f, glm::vec3(1.0, 0.0, 0.0)) * glm::scale(glm::mat4(1.0), glm::vec3(WINDOW_WIDTH, 1, WINDOW_HEIGHT)));
}
void View::openFile(string& filename)
{
  if (graph != NULL)
  {
    delete graph;
    graph = NULL;
  }
  graph = ObjectXMLReader::readObjectXMLFile(filename);
  if (shaderProgram != 0)
    graph->initShaderProgram(shaderProgram);
}
void View::initialize()
{
  ShaderInfo shaders[] =
  {
    {GL_VERTEX_SHADER, "lighting-texturing.vert"},
    {GL_FRAGMENT_SHADER, "lighting-texturing.frag"},
    {GL_NONE, ""}
  };
  shaderProgram = createShaders(shaders);
  shaders[0].filename = "bare.vert";
  shaders[1].filename = "bare.frag";
  rtProgram = createShaders(shaders);
  projectionLocation = glGetUniformLocation(shaderProgram, "projection");
  rtProjLocation = glGetUniformLocation(rtProgram, "projection");
  rtModelViewLocation = glGetUniformLocation(rtProgram, "modelview");
  rtTextureLocation = glGetUniformLocation(rtProgram, "image");
  rtPlane = new Plane(NULL);
  rtPlane->setTransform(glm::scale(glm::mat4(1.0), glm::vec3(WINDOW_WIDTH, 1, WINDOW_HEIGHT)));
  worldToView = glm::lookAt(glm::vec3(400, 400, 400), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}
void View::draw()
{
  if (mode == OPENGL)
  {
    glUseProgram(shaderProgram);
    modelview.push(modelview.top());
    modelview.top() = modelview.top() * worldToView;
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(proj.top()));
    if (graph != NULL)
    {
      graph->enableLights(modelview);
      graph->draw(modelview);
    }
    modelview.pop();
    glFinish();
  }
  else
  {
    if (rtTextureID == -1)
    {
      float* buffer;
      modelview.push(modelview.top());
      modelview.top() = modelview.top() * worldToView;
      buffer = graph->raytrace(WINDOW_WIDTH, WINDOW_HEIGHT, modelview);
      modelview.pop();
      glGenTextures(1, &rtTextureID);
      glBindTexture(GL_TEXTURE_2D, rtTextureID);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_FLOAT, buffer);
      delete []buffer;
      glBindTexture(GL_TEXTURE_2D, 0);
    }
    glUseProgram(rtProgram);
    proj.push(proj.top());
    proj.top() = glm::ortho(-0.5f * WINDOW_WIDTH, 0.5f * WINDOW_WIDTH, -0.5f * WINDOW_HEIGHT, 0.5f * WINDOW_HEIGHT, 0.1f, 10000.0f);
    modelview.push(glm::lookAt(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
    modelview.top() = modelview.top() * rtPlane->getTransform();
    glUniformMatrix4fv(rtProjLocation, 1, GL_FALSE, glm::value_ptr(proj.top()));
    glUniformMatrix4fv(rtModelViewLocation, 1, GL_FALSE, glm::value_ptr(modelview.top()));
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, rtTextureID);
    glUniform1i(rtTextureLocation, 0);
    rtPlane->draw(modelview);
    modelview.pop();
    proj.pop();
    glFinish();
  }
  glUseProgram(0);
}
void View::animate(float time)
{
  if (graph != NULL)
    graph->animate(time);
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
      return false;
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
      for (ShaderInfo* processed = shaders; processed->type != GL_NONE; processed++)
      {
        glDeleteShader(processed->shader);
        processed->shader = 0;
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
    for (ShaderInfo* processed = shaders; processed->type != GL_NONE; processed++)
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
    *major = *minor = 0;
}
void View::getGLSLVersion(int* major, int* minor)
{
  int gl_major, gl_minor;
  getOpenGLVersion(&gl_major, &gl_minor);
  *major = *minor = 0;
  if (gl_major == 1)
  {
    const char* extstr = (const char*)glGetString(GL_EXTENSIONS);
    if ((extstr != NULL) && (strstr(extstr, "GL_ARB_shading_language_100") != NULL))
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
