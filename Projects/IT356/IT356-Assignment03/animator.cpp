#include "animator.h"


Animator::Animator()
{
    this->rotationAngle = 0.0;
}

void Animator::increaseAngle()
{
    this->rotationAngle += 0.1f;
}

glm::mat4& Animator::yes(string& name, glm::mat4& transformationMatrix)
{
    //If it is a face, make it nod
    if(name.compare("face")==0)
    {
        //rotate it along the x axis in a harmonic motion
        transformationMatrix = glm::rotate(glm::mat4(1.0), (30 * sin(rotationAngle)), glm::vec3(1, 0, 0)) * transformationMatrix;
    }
    return transformationMatrix;
}

glm::mat4& Animator::animateThis(string& name, glm::mat4& transformationMatrix)
{
    //If it is a propeller
    if(name.compare("propeller")==0)
    {
        //rotate it very quickly along the z axis
        transformationMatrix = glm::rotate(glm::mat4(1.0), rotationAngle*200, glm::vec3(0,0,1)) * transformationMatrix;
    }
    //If they are rear wheels
    if(name.compare("rear wheels")==0)
    {
        //translate the rear wheels back to the origin, apply a rotation along the x axis, and translate them back to their original spot
        transformationMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0,10,30)) * transformationMatrix;
        transformationMatrix = glm::rotate(glm::mat4(1.0), rotationAngle*50, glm::vec3(1,0,0)) * transformationMatrix;
        transformationMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0,-10,-30)) * transformationMatrix;
    }
    //If they are front wheels
    if(name.compare("front wheels")==0)
    {
        //translate them to the origin, apply a rotation along the x axis, and translate them back
        transformationMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0,18,0)) * transformationMatrix;
        transformationMatrix = glm::rotate(glm::mat4(1.0), rotationAngle*50, glm::vec3(1,0,0)) * transformationMatrix;
        transformationMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0,-18,0)) * transformationMatrix;
    }
    //If it is the entire airplane
    if(name.compare("airplane")==0)
    {
        //Apply a 45 degree bank to the plane, translate it out to the edge of the arc, and rotate it along the x-z plane
        transformationMatrix = glm::rotate(glm::mat4(1.0), 45.0f , glm::vec3(0,0,1)) * transformationMatrix;
        transformationMatrix = glm::translate(glm::mat4(1.0), glm::vec3(350,0,0)) * transformationMatrix;
        transformationMatrix = glm::rotate(glm::mat4(1.0), -rotationAngle, glm::vec3(0,1,0)) * transformationMatrix;
    }
    return transformationMatrix;
}
