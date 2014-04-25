/*
 * Assignment 3 - IT356 Fall 2013
 * Locomotive Animation
 * By Thomas Lyons
 */

#include <QCoreApplication>
#include "View.h"
using namespace std;
/*void mouseButton(int button, int state, int x, int y);
void mouseMove(int x, int y);
//change in x
float deltaXAngle = 0.0;
//change in y
float deltaYAngle = 0.0;
//dragging flag
int isDragging = 0;
//saved x position
int prevX = 0;
//saved y position
int prevY = 0;*/
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
    //define a function to run upon clicking the mouse button
    //glutMouseFunc(mouseButton);
    //define a function to run upon moving the mouse
    //glutMotionFunc(mouseMove);

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
    glEnable(GL_DEPTH_TEST);
    //feed the change in angle to the draw method
    v.draw();
    //v.draw(deltaXAngle, deltaYAngle);
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

/*void mouseButton(int button, int state, int x, int y)
{
    //if the left mouse button is active
    if(button == GLUT_LEFT_BUTTON)
    {
        //if the left mouse button is pressed down
        if (state == GLUT_DOWN)
        {
            //enable dragging
            isDragging = 1;
            //save the x position of the mouse
            prevX = x;
            //save the y position of the mouse
            prevY = y;
        }
        //if the left mouse button has been released
        else
        {
            //disable dragging
            isDragging = 0;
        }
    }
}

void mouseMove(int x, int y)
{
    //if dragging is enabled (left mouse button down)
    if(isDragging)
    {
        //the change in the x angle is the change in the x position from the last time the mouse button was pressed down or moved scaled down for convenience
        deltaXAngle = (x-prevX) * .5;
        //save the x position
        prevX = x;
        //the change in the x angle is the change in the x position from the last time the mouse button was pressed down or moved scaled down for convenience
        deltaYAngle = (y-prevY) * .5;
        //save the y position
        prevY = y;
        //redisplay
        glutPostRedisplay();
    }
}*/
