#include <QCoreApplication>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "View.h"
#include <iostream>
using namespace std;
void init();
void display();
void keyboard(unsigned char key, int x, int y);
void resize(int w, int h);
void idle();
View v;
typedef struct
{
  int frames;
  int timeBase;
  int timeElapsed;
  float framerate;
} FrameRateMeasurementInfo;
enum MENUCODES {FILE_OPEN};
string filename;
float time;
FrameRateMeasurementInfo fpsInfo;
int main(int argc, char* argv[])
{
  QCoreApplication a(argc, argv);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(600, 600);
  glutInitWindowPosition(400, 400);
  glutInitContextVersion(4, 0);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutCreateWindow("Raytracer");
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    cerr << "Unable to initialize GLEW...exiting" << endl;
    exit(1);
  }
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyboard);
  glutIdleFunc(idle);
  glutMainLoop();
  return a.exec();
}
void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
    case 'r':
    case 'R':
      v.initRaytraceMode();
      glutPostRedisplay();
      break;
    case 'o':
    case 'O':
      v.initOpenGLMode();
      glutPostRedisplay();
      break;
  }
}
void display()
{
  string frameRateString = "Frame rate: ";
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  v.draw();
  glutSwapBuffers();
}
void idle()
{
  if (time > 7.2)
    time = time - 7.2f;
  else
    time += 0.001f;
  fpsInfo.frames++;
  fpsInfo.timeElapsed = glutGet(GLUT_ELAPSED_TIME);
  if (fpsInfo.timeElapsed - fpsInfo.timeBase > 1000)
  {
    fpsInfo.framerate = fpsInfo.frames * 1000.0 / (fpsInfo.timeElapsed - fpsInfo.timeBase);
    fpsInfo.timeBase = fpsInfo.timeElapsed;
    fpsInfo.frames = 0;
  }
  v.animate(time);
  cout << "Frame rate: " << fpsInfo.framerate << "\r";
  glutPostRedisplay();
}
void resize(int w, int h)
{
  v.resize(w, h);
  glViewport(0, 0, w, h);
  glutPostRedisplay();
}
void init()
{
  glClearColor(0, 0, 0, 1);
  v.initialize();
  string s;
  s = "fermatspiral.txt";
  v.openFile(s);
  time = 0;
  fpsInfo.timeBase = fpsInfo.timeElapsed = 0;
  fpsInfo.frames = 0;
}
