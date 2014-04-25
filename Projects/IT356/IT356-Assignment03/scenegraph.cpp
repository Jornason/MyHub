#include "scenegraph.h"

Scenegraph::Scenegraph(GLint objectColorLocation, GLint modelviewLocation, glm::mat4& modelview)
{
    this->objectColorLocation = objectColorLocation;
    this->modelviewLocation = modelviewLocation;
    this->modelview = modelview;
    this->animation = new Animator();
}

void Scenegraph::draw(stack<glm::mat4> &transformationstack)
{
    //get the latest transformation from the stack.
    glm::mat4 latesttransformation = transformationstack.top();
    //clean out the stack of transformations
    while(!transformationstack.empty())
    {
        transformationstack.pop();
    }
    //modify the transformation with the modelview and push it to the stack
    transformationstack.push(modelview * latesttransformation);
    //draw the scene
    root->draw(transformationstack);
}

void Scenegraph::increaseAngle()
{
    //call the animator's increasAngle() function
    this->animation->increaseAngle();
}

Scenegraph::~Scenegraph()
{
    delete animation;
    delete root;
}

Group* Scenegraph::getRoot()
{
    return root;
}

void Scenegraph::setRoot(Group* root)
{
    this->root = root;
}

GLint Scenegraph::getObjectColorLocation()
{
    return objectColorLocation;
}

void Scenegraph::setObjectColorLocation(GLint objectColorLocation)
{
    this->objectColorLocation = objectColorLocation;
}

GLint Scenegraph::getModelviewLocation()
{
    return modelviewLocation;
}

void Scenegraph::setModelviewLocation(GLint modelviewLocation)
{
    this->modelviewLocation = modelviewLocation;
}

glm::mat4& Scenegraph::getModelview()
{
    return modelview;
}

void Scenegraph::setModelview(glm::mat4& modelview)
{
    this->modelview = modelview;
}

stack<Group*>& Scenegraph::getCurrentGroup()
{
    return currentGroup;
}

void Scenegraph::setCurrentGroup(stack<Group*>& currentGroup)
{
    this->currentGroup = currentGroup;
}

Animator* Scenegraph::getAnimation()
{
    return animation;
}

void Scenegraph::setAnimation(Animator* animation)
{
    this->animation = animation;
}


