/*
 * Assignment 3 - IT356 Fall 2013
 * Locomotive Animation
 * By Thomas Lyons
 */

#include <QCoreApplication>
#include "View.h"
using namespace std;

//Initializer function
void init();
//Display function
void display();
//Window resize function
void resize(int w,int h);
//Idle Function
void idle();
//View object
View v;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(400,400);
    glutInitWindowPosition(400,400);
    glutCreateWindow("Hello world");
    glewExperimental = GL_TRUE;
    if (glewInit()!=GLEW_OK)
    {
        cerr << "Unable to initialize GLEW...exiting" << endl;
        exit(1);
    }
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutIdleFunc(idle);
    glutMainLoop();
    return a.exec();
}

void idle()
{
    //animate and redisplay
    v.animate();
    glutPostRedisplay();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLdouble eqn[4] = {0.0, 1.0, 0.0, 0.0};
    glClipPlane(GL_CLIP_PLANE0, eqn);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CLIP_PLANE0);
    //feed the change in angle to the draw method
    v.draw();
    glutSwapBuffers();
}

void resize(int w,int h)
{
    v.resize(w,h);
    glViewport(0,0,w,h);
    glutPostRedisplay();
}

void init()
{
    int major,minor;
    v.getOpenGLVersion(&major,&minor);
    cout << "OpenGL version supported by the GLUT window: " << major << "." << minor << endl;
    v.getGLSLVersion(&major,&minor);
    cout << "GLSL version supported by the GLUT window: " << major << "." << minor << endl;
    glClearColor(0,0,0,1);
    v.initialize();
}
