#include "View.h"
#include <GL/glew.h>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <time.h>
using namespace std;
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
struct tm* newtime;
time_t ltime;
View::View()
  : QuadNumVertices(4), CircleNumVertices(6)
{
}
void View::resize(int w, int h)
{
  WINDOW_WIDTH = w;
  WINDOW_HEIGHT = h;
  proj = glm::ortho(-WINDOW_WIDTH / 2.0f, WINDOW_WIDTH / 2.0f, -WINDOW_HEIGHT / 2.0f, WINDOW_HEIGHT / 2.0f);
}
void View::initializeCircle()
{
  ShaderInfo shaders[] =
  {
    {GL_VERTEX_SHADER, "trianglesV1.vert"},
    {GL_FRAGMENT_SHADER, "triangles.frag"},
    {GL_NONE, ""}
  };
  GLuint program = createShaders(shaders);
  glUseProgram(program);
  projectionLocation = glGetUniformLocation(program, "projection");
  modelViewLocation = glGetUniformLocation(program, "modelview");
  vZColorNew = glGetAttribLocation(program, "vColor");
  vector<VertexAttribs> vertexdata;
  int SLICES = 360;
  \
  VertexAttribs v;
  v.position[0] = v.position[1] = v.position[2] = 0;
  v.position[3] = 1;
  vertexdata.push_back(v);
  float PI = 3.14159f;
  for (int i = 0; i <= SLICES; i++)
  {
    float theta;
    theta = (float)i * 2 * PI / SLICES;
    v.position[0] = 1 * cos(theta);
    v.position[1] = 1 * sin(theta);
    v.position[2] = 0;
    v.position[3] = 1;
    vertexdata.push_back(v);
  }
  v.position[0] = 1;
  v.position[1] = v.position[2] = 0;
  v.position[3] = 1;
  vertexdata.push_back(v);
  vector<GLuint> indices;
  for (GLuint i = 0; i < vertexdata.size(); i++)
    indices.push_back(i);
  numIndices = indices.size();
  glGenVertexArrays(1, &vaoCircle);
  glBindVertexArray(vaoCircle);
  glGenBuffers(NumBuffers, vboCircle);
  glBindBuffer(GL_ARRAY_BUFFER, vboCircle[ArrayBuffer]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VertexAttribs)*vertexdata.size(), &vertexdata[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboCircle[IndexBuffer]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vboCircle[ArrayBuffer]);
  glVertexAttribPointer(vPositionNew, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAttribs), BUFFER_OFFSET(0));
  glEnableVertexAttribArray(vPositionNew);
  glBindVertexArray(0);
}
void View::initializeQuad()
{
  ShaderInfo shaders[] =
  {
    {GL_VERTEX_SHADER, "trianglesV1.vert"},
    {GL_FRAGMENT_SHADER, "triangles.frag"},
    {GL_NONE, ""}
  };
  GLuint program = createShaders(shaders);
  glUseProgram(program);
  vZColorNew = glGetAttribLocation(program, "vColor");
  projectionLocation = glGetUniformLocation(program, "projection");
  modelViewLocation = glGetUniformLocation(program, "modelview");
  GLfloat vertices[][4] =
  {
    { -0.5f, 0.0f, 0, 1.0f},
    { -0.5f, 1.0f, 0, 1.0f},
    {0.5f, 1.0f, 0, 1.0f},
    {0.5f, 0.0f, 0, 1.0f}
  };
  vector<GLuint> indices;
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(2);
  indices.push_back(3);
  vector<VertexAttribs> vertexdata;
  for (int i = 0; i < 4; i++)
  {
    VertexAttribs v;
    vertexdata.push_back(v);
    for (int j = 0; j < 4; j++)
      vertexdata[i].position[j] = vertices[i][j];
  }
  glGenVertexArrays(1, &vaoQuad);
  glBindVertexArray(vaoQuad);
  glGenBuffers(NumBuffers, vboQuad);
  glBindBuffer(GL_ARRAY_BUFFER, vboQuad[ArrayBuffer]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VertexAttribs)*vertexdata.size(), &vertexdata[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboQuad[IndexBuffer]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vboQuad[ArrayBuffer]);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAttribs), BUFFER_OFFSET(0));
  glEnableVertexAttribArray(vPosition);
  glBindVertexArray(0);
}
void View::draw()
{
  //Antialiasing
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
  //Outer ring
  modelView.push(glm::mat4(1.0));
  glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(proj));
  //Paint the outer ring black
  glVertexAttrib3f(vZColorNew, 0.0f, 0.0f, 0.0f);
  //scale the outer ring to have a radius of 300 pixels
  modelView.top() = glm::scale(glm::mat4(1.0), glm::vec3(300, 300, 1));
  //bind the vertex array
  glBindVertexArray(vaoCircle);
  glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, glm::value_ptr(modelView.top()));
  //Draw the circle
  glDrawElements(GL_TRIANGLE_FAN, numIndices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
  //draw the line around it for antialiasing
  glDrawElements(GL_LINE_LOOP, numIndices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
  //Dial
  //Make the dial cream colored
  glVertexAttrib3f(vZColorNew, 1.0f, 1.0f, 0.8f);
  //scale the dial to have a radius of 270 pixels
  modelView.top() = glm::scale(glm::mat4(1.0), glm::vec3(270, 270, 1));
  glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, glm::value_ptr(modelView.top()));
  glDrawElements(GL_TRIANGLE_FAN, numIndices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
  glDrawElements(GL_LINE_LOOP, numIndices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
  //Draw the 15 minute ticks
  //Paint the 15 minute ticks black
  glVertexAttrib3f(vZColorNew, 0.0f, 0.0f, 0.0f);
  //Make the 15 minute ticks 8 by 15 pixels
  modelView.top() = glm::scale(glm::mat4(1.0), glm::vec3(8, 50, 1));
  //Move the ticks to the appropriate point
  modelView.top() = glm::translate(glm::mat4(1.0), glm::vec3(0, 185, 0)) * modelView.top();
  glBindVertexArray(vaoQuad);
  for (int i = 0; i < 4; i++)
  {
    modelView.push(modelView.top());
    //Rotate 90 degrees each time
    modelView.top() = glm::rotate(glm::mat4(1.0), 90.0f * i, glm::vec3(0, 0, 1)) * modelView.top();
    glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, glm::value_ptr(modelView.top()));
    glDrawElements(GL_QUADS, numIndices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    glDrawElements(GL_LINE_LOOP, numIndices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    modelView.pop();
  }
  //Draw the 5 minute ticks
  //5 by 30 pixels
  modelView.top() = glm::scale(glm::mat4(1.0), glm::vec3(5, 30, 1));
  modelView.top() = glm::translate(glm::mat4(1.0), glm::vec3(0, 205, 0)) * modelView.top();
  for (int i = 0; i < 12; i++)
  {
    modelView.push(modelView.top());
    //rotate 30 degrees each time
    modelView.top() = glm::rotate(glm::mat4(1.0), 30.0f * i, glm::vec3(0, 0, 1)) * modelView.top();
    glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, glm::value_ptr(modelView.top()));
    glDrawElements(GL_QUADS, numIndices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    glDrawElements(GL_LINE_LOOP, numIndices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    modelView.pop();
  }
  //Draw the 1 minute ticks
  //3 by 20 pixels
  modelView.top() = glm::scale(glm::mat4(1.0), glm::vec3(3, 20, 1));
  modelView.top() = glm::translate(glm::mat4(1.0), glm::vec3(0, 215, 0)) * modelView.top();
  for (int i = 0; i < 60; i++)
  {
    modelView.push(modelView.top());
    //rotate 6 degrees each time
    modelView.top() = glm::rotate(glm::mat4(1.0), 6.0f * i, glm::vec3(0, 0, 1)) * modelView.top();
    glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, glm::value_ptr(modelView.top()));
    glDrawElements(GL_QUADS, numIndices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    glDrawElements(GL_LINE_LOOP, numIndices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    modelView.pop();
  }
  //draw hour hand
  //12 by 120 pixels
  modelView.top() = glm::scale(glm::mat4(1.0), glm::vec3(12, 120, 1));
  //set the angle
  modelView.top() = glm::rotate(glm::mat4(1.0), hourangle, glm::vec3(0, 0, 1)) * modelView.top();
  glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, glm::value_ptr(modelView.top()));
  glDrawElements(GL_QUADS, numIndices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
  glDrawElements(GL_LINE_LOOP, numIndices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
  //draw minute hand
  //12 by 235 pixels
  modelView.top() = glm::scale(glm::mat4(1.0), glm::vec3(12, 235, 1));
  //set the angle
  modelView.top() = glm::rotate(glm::mat4(1.0), minangle, glm::vec3(0, 0, 1)) * modelView.top();
  glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, glm::value_ptr(modelView.top()));
  glDrawElements(GL_QUADS, numIndices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
  glDrawElements(GL_LINE_LOOP, numIndices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
  //draw second hand
  //red
  glVertexAttrib3f(vZColorNew, 1.0f, 0.0f, 0.0f);
  //6 by 235 pixels
  modelView.top() = glm::scale(glm::mat4(1.0), glm::vec3(6, 235, 1));
  //set the angle
  modelView.top() = glm::rotate(glm::mat4(1.0), secangle, glm::vec3(0, 0, 1)) * modelView.top();
  glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, glm::value_ptr(modelView.top()));
  glDrawElements(GL_QUADS, numIndices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
  glDrawElements(GL_LINE_LOOP, numIndices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
  //Draw the red circle in the middle
  //7 pixel radius
  modelView.top() = glm::scale(glm::mat4(1.0), glm::vec3(7, 7, 1));
  glBindVertexArray(vaoCircle);
  glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, glm::value_ptr(modelView.top()));
  glDrawElements(GL_TRIANGLE_FAN, numIndices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
  glDrawElements(GL_LINE_LOOP, numIndices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
  modelView.pop();
  glFlush();
}
void View::animate()
{
  //get the time
  time(&ltime);
  newtime = localtime(&ltime);
  //set the angles
  secangle = -((360 / 60) * newtime->tm_sec);
  minangle = -((6 * newtime->tm_min) + (0.1f * newtime->tm_sec));
  hourangle = -((30 * newtime->tm_hour) + (0.5f * newtime->tm_min) + (.0083333333 * newtime->tm_sec));
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
  //  printOpenGLError();
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
    /* GL v1.x can only provide GLSL v1.00 as an extension */
    const char* extstr = (const char*)glGetString(GL_EXTENSIONS);
    if ((extstr != NULL) && (strstr(extstr, "GL_ARB_shading_language_100") != NULL))
    {
      *major = 1;
      *minor = 0;
    }
  }
  else if (gl_major >= 2)
  {
    /* GL v2.0 and greater must parse the version string */
    const char* verstr = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
    if ((verstr == NULL) || (sscanf_s(verstr, "%d.%d", major, minor) != 2))
    {
      *major = 0;
      *minor = 0;
    }
  }
}
