#include "Object.h"

Object::Object(string name, Animator* animation)
    : Node()
{
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    glGenBuffers(NumBuffers,buffers);
    setName(name);
    this->animation = animation;
}

Object::~Object(void)
{
    //delete the animation
    delete animation;
    if (VAO!=0)
    {
        //clear the buffers
        glDeleteBuffers(NumBuffers,buffers);
        //cleare the VAO
        glDeleteVertexArrays(1,&VAO);
    }
}

void Object::draw(stack<glm::mat4> &transformation)
{
    //Get the object's color
    glm::vec3 color = this->getColor();
    //tell the shader where the color is
    glVertexAttrib3fv(scene->getObjectColorLocation(),glm::value_ptr(color));
    //get the transformation matrix
    glm::mat4 currentTransformationMatrix = getTransform();
    //Apply the animations
    currentTransformationMatrix = this->animation->animateThis(getName(), currentTransformationMatrix);
    currentTransformationMatrix = this->animation->yes(getName(), currentTransformationMatrix);
    //Apply the previous transformations
    currentTransformationMatrix = transformation.top() * currentTransformationMatrix;
    //push the object's transformation to the stack
    transformation.push(currentTransformationMatrix);
    //tell the shader where the transformation is
    glUniformMatrix4fv(scene->getModelviewLocation(),1,GL_FALSE,glm::value_ptr(transformation.top()));
    //bind the VAO
    glBindVertexArray(VAO);
    //draw
    glDrawElements(GL_TRIANGLES,triangleIndices.size(),GL_UNSIGNED_INT,BUFFER_OFFSET(0));
    //pop the transformation
    transformation.pop();
}


void Object::setColor(float r, float g, float b)
{
    color = glm::vec3(r,g,b);
}

glm::vec3& Object::getColor()
{
    return color;
}

Animator* Object::getAnimation()
{
    return animation;
}

void Object::setAnimation(Animator* animation)
{
    this->animation = animation;
}

