#ifndef ANIMATOR_H
#define ANIMATOR_H
#include "gl/glut.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string.h>
#include <math.h>
using namespace std;

/*
 * Animator Class
 * This class takes care of all of the responsibility of animation for the program.
 * New animation routines can be defined here.
 */

class Animator
{
private:
    //The variable to use in the transformations
    float rotationAngle;
public:
    //Default constructor
    Animator();
    //Increase the variable that is fed into the animation
    void increaseAngle();
    //Nod the head
    glm::mat4& yes(string& name, glm::mat4& transformationMatrix);
    //Animate the plane
    glm::mat4& animateThis(string& name, glm::mat4& transformationMatrix);
};
#endif // ANIMATOR_H
