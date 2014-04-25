#include<QCoreApplication>
#include<GL/glew.h>
#include<GL/freeglut.h>
#include"View.h"
#include<iostream>
#include"mesh.h"
using namespace std;
void init();
void display();
void resize(int w, int h);
void mouseButton(int button, int state, int x, int y);
void mouseMove(int x, int y);
void mouseWheel(int button, int dir, int x, int y);
void keyboardSpecial(int key, int x, int y);
void keyboard(unsigned char key, int x, int y);
float deltaXAngle = 0.0, deltaYAngle = 0.0;
bool isDragging = 0;
int prevX = 0, prevY = 0;
double tolerance = 0.0;
int orientation = 0;
double scale = 1.0;
View v;
int main(int argc, char* argv[])
{
  QCoreApplication a(argc, argv);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(400, 400);
  glutInitWindowPosition(400, 400);
  glutCreateWindow("Hello world");
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    cerr << "Unable to initialize GLEW...exiting" << endl;
    exit(1);
  }
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMouseFunc(mouseButton);
  glutMotionFunc(mouseMove);
  glutSpecialFunc(keyboardSpecial);
  glutKeyboardFunc(keyboard);
  glutMouseWheelFunc(mouseWheel);
  glutMainLoop();
  return a.exec();
}
void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  v.draw(deltaXAngle, deltaYAngle, tolerance, scale);
  glutSwapBuffers();
}
void resize(int w, int h)
{
  v.resize(w, h);
  glViewport(0, 0, w, h);
  glutPostRedisplay();
}
void init()
{
  int mj, mn;
  v.getOpenGLVersion(&mj, &mn);
  cout << "OpenGL version supported: " << mj << "." << mn << endl;
  v.getGLSLVersion(&mj, &mn);
  cout << "GLSL version supported: " << mj << "." << mn << endl;
  glClearColor(0, 0, 0, 1);
  v.name = "teddybear";
  v.filename = "teddybear.obj.txt";
  v.initialize(orientation);
}
void mouseButton(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON)
  {
    if (state == GLUT_DOWN)
    {
      isDragging = 1;
      prevX = x;
      prevY = y;
    }
    else
    {
      isDragging = 0;
    }
  }
}
void mouseMove(int x, int y)
{
  if (isDragging)
  {
    deltaXAngle = (x - prevX) * .5;
    prevX = x;
    deltaYAngle = (y - prevY) * .5;
    prevY = y;
    scale = 1.0;
    glutPostRedisplay();
  }
}
void keyboardSpecial(int key, int x, int y)
{
  deltaXAngle = 0.0;
  deltaYAngle = 0.0;
  scale = 1.0;
  switch (key)
  {
    case GLUT_KEY_UP:
      tolerance += 0.05;
      glutPostRedisplay();
      break;
    case GLUT_KEY_DOWN:
      tolerance -= 0.5;
      glutPostRedisplay();
      break;
  }
}
void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
    case '0':
      orientation = 0;
      break;
    case '1':
      orientation = 1;
      break;
    case '2':
      orientation = 2;
      break;
    case '3':
      orientation = 3;
      break;
    case '4':
      orientation = 4;
      break;
    case '5':
      orientation = 5;
      break;
  }
  deltaXAngle = 0.0;
  deltaYAngle = 0.0;
  scale = 1.0;
  for (unsigned int i = 0; i < v.objectsList.size(); i++)
  {
    delete v.objectsList[i];
  }
  cout << "orientation is " << orientation << endl;
  v.objectsList.clear();
  Mesh* model = new Mesh(v.name, v.filename, orientation);
  model->setTransform(glm::mat4(1.0));
  v.objectsList.push_back(model);
  glutPostRedisplay();
}
void mouseWheel(int button, int dir, int x, int y)
{
  deltaXAngle = 0.0;
  deltaYAngle = 0.0;
  if (dir > 0)
  {
    scale = 1.1;
    glutPostRedisplay();
  }
  else
  {
    scale = 0.9;
    glutPostRedisplay();
  }
}
