#include<QCoreApplication>
#include<GL/glew.h>
#include<GL/freeglut.h>
#include"View.h"
#include<iostream>
#include"mesh.h"
using namespace std;

//Compute the stability of a 3D printable model and split as necessary to ensure
//a clean print

//The constructor for the mesh reads in a .obj file and computes the stability
//of the model given a material tolerance provided by the user. If a model has
//unstable parts, they are split into a new model and the process repeats.

//Initially, tolerance is set to 1.0. It can be increased or decreased with the
//up and down arrow keys. The model will reorient itself as this value changes
//to display the model with the most supported vertices given the tolerance.

//The model can also be rotated and the user can zoom in and out

void init();
void display();
void resize(int w, int h);
void mouseButton(int button, int state, int x, int y);
void mouseMove(int x, int y);
void mouseWheel(int button, int dir, int x, int y);
void keyboardSpecial(int key, int x, int y);
float deltaXAngle = 0.0, deltaYAngle = 0.0;
bool isDragging = 0;
int prevX = 0, prevY = 0;
double tolerance = 1.0;
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
  glutMouseWheelFunc(mouseWheel);
  glutMainLoop();
  return a.exec();
}

void display()
{
  //Clear the display and rerender
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  v.draw(deltaXAngle, deltaYAngle, tolerance, scale);
  glutSwapBuffers();
}

void resize(int w, int h)
{
  //Resize the window and redisplay
  v.resize(w, h);
  glViewport(0, 0, w, h);
  glutPostRedisplay();
}

void init()
{
  //Initialize the display window
  int mj, mn;
  v.getOpenGLVersion(&mj, &mn);
  cout << "OpenGL version supported: " << mj << "." << mn << endl;
  v.getGLSLVersion(&mj, &mn);
  cout << "GLSL version supported: " << mj << "." << mn << endl;
  glClearColor(0, 0, 0, 1);
  //Specify the mesh to process
  v.filename = "teddybear.obj";
  v.initialize();
}

//These two methods control clicking and dragging to rotate the model
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

//Increase or decrease the tolerance of the model
void keyboardSpecial(int key, int, int)
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
      tolerance -= 0.05;
      glutPostRedisplay();
      break;
  }
  cout << "tolerance = " << tolerance << endl;
}

//Zoom in or out
void mouseWheel(int, int dir, int, int)
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
