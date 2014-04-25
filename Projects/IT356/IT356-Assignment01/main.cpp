/*
 * This is the main file. It sets up the window using glut and initializes OpenGL extensions using glew
 *
 * We use freeglut, an open-source library inspired from the original glut. GLUT (and freeglut) is a cross-platform OpenGL library for opening windows and other UI elements
 * It is by no means a full-fledged powerful UI framework, but it is very simple to use, and is cross-platform.
 * Feel free to use and modify this file for every project.
 *
 * glut and indeed opengl, were written for C, and as such have no inherent support for objects. That is why this file looks like a C program.
 *
 * In order to initialize OpenGL extensions to use OpenGL 4, we use glew. This simplifies setting it up for every program.
 **/
#include <QCoreApplication> //the Qt include, not being used right now for anything
//You will find this header until C:\QTFiles, so include that path in the project file
#include <GL/glew.h> //the header for the glew library
#include <GL/freeglut.h> //the header for the glut library
#include "View.h"
#include <iostream>
using namespace std;
/* function prototypes */
void init(); //the function that will be used to initialize stuff
void display(); //the function is called every time the screen is to be refreshed
void resize(int w, int h); //the function that is called every time the window is manually or automatically resized
void idle();
View v; //an object to our View class that encapsulates everything that we do.
int main(int argc, char* argv[])
{
  QCoreApplication a(argc, argv); //standard Qt stuff, do not change
  //initialize the GLUT environment, standard for all glut programs
  glutInit(&argc, argv);
  //we want a glut window that is double-buffered and supports red, green, blue and alpha (transparency)
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  //the initial size of the window
  glutInitWindowSize(600, 600);
  //the position of the top left corner of the window on the screen
  glutInitWindowPosition(400, 400);
  //this call finally creates and shows the window with the provided title.
  //Important: No OpenGL command before this line will work!.
  glutCreateWindow("Analog Clock");
  //glew currently has a bug that causes problems with 4.0. Setting this glew flag is the temporary solution for it
  glewExperimental = GL_TRUE;
  //initialize glew which initializes all the OpenGL extensions.
  if (glewInit() != GLEW_OK)
  {
    cerr << "Unable to initialize GLEW...exiting" << endl;
    exit(1);
  }
  //initialize stuff. This will likely change with every program.
  init();
  //tell glut which function to call in order to draw the screen
  //You can name the drawing function whatever you want, so long as it does not take any parameters and returns void
  glutDisplayFunc(display);
  //tell glut which function to call if the window resizes
  //You can name the resizing function whatever you want, so long as it takes two integer parameters (width and height of the resized window) and returns void
  glutReshapeFunc(resize);
  //Animate it
  glutIdleFunc(idle);
  //this starts the main (and infinite) loop that listens to events, refreshes, whatever
  glutMainLoop();
  //at this step you would clean up any memory, etc.
  return a.exec(); //standard Qt stuff, do not change
}
void idle()
{
  //Recalculate the angles of the clock hands
  v.animate();
  //redisplay
  glutPostRedisplay();
}
void display()
{
  //Paint the background the same color as vomit
  glClearColor(0.6667, 0.5098, 0.2, 1);
  glClear(GL_COLOR_BUFFER_BIT); //this command actually clears the window.
  v.draw(); //simply delegate to our view class that has all the data and does all the rendering
  glutSwapBuffers(); //since we have double-buffering, all the above drawing is done to an invisible back-buffer. This command swaps the buffers so that everything you drew is now visible.
}
void resize(int w, int h)
{
  //delegate to our view class.
  v.resize(w, h);
  //sets the viewport to cover the entire area of the resized window
  glViewport(0, 0, w, h);
  glutPostRedisplay();
}
void init()
{
  int major, minor;
  v.getOpenGLVersion(&major, &minor);
  cout << "Opengl version supported : " << major << "." << minor << endl;
  v.getGLSLVersion(&major, &minor);
  cout << "GLSL version supported : " << major << "." << minor << endl;
  //delegate to our view class to do all the initializing
  //Initialize the quad
  v.initializeQuad();
  //Initialize the circle
  v.initializeCircle();
}
