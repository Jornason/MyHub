#include "group.h"

Group::Group(string name, Scenegraph* scene, Animator* animation)
    : Node(name, scene)
{
    setName(name);
    setTransform(glm::mat4(1.0));
    this->scene = scene;
    this->animation = animation;
}

void Group::addChild(Node* child)
{
    //push a child onto the vector
    children.push_back(child);
}

void Group::draw(stack<glm::mat4> &transformation, GLuint program)
{
    //Get the transformation for this group
    glm::mat4 currentTransformationMatrix = getTransform();
    //Apply animations to it
    currentTransformationMatrix = this->animation->animateThis(getName(), currentTransformationMatrix);
    currentTransformationMatrix = this->animation->yes(getName(), currentTransformationMatrix);
    //Apply all of the previous transformations to it
    currentTransformationMatrix = transformation.top() * currentTransformationMatrix;
    //Push it to the stack of transformations
    transformation.push(currentTransformationMatrix);
    //draw all of the children using the modified transformation stack
    for(unsigned int i = 0; i < children.size(); i++)
    {
        children[i]->draw(transformation, program);
    }
    //pop the transformation from the stack after drawing the children
    transformation.pop();
}

vector<Node*>& Group::getChildren()
{
    return children;
}

void Group::setChildren(vector<Node*> &children)
{
    this->children = children;
}

Animator* Group::getAnimation()
{
    return animation;
}

void Group::setAnimation(Animator* animation)
{
    this->animation = animation;
}

Group::~Group()
{
    delete animation;
}


vector<Light>& Group::getLights(vector<Light>& lightListCopy, stack<glm::mat4>& transformation)
{
    glm::mat4 currentTransformationMatrix = getTransform();
    //Apply animations to it
    currentTransformationMatrix = this->animation->animateThis(getName(), currentTransformationMatrix);
    currentTransformationMatrix = this->animation->yes(getName(), currentTransformationMatrix);
    //Apply all of the previous transformations to it
    //glm::vec4 currentTransformationVector = transformation.top() * currentTransformationMatrix[3];
    currentTransformationMatrix = transformation.top() * currentTransformationMatrix;
    //Push it to the stack of transformations
    transformation.push(currentTransformationMatrix);
    unsigned int i = 0;
    for(i = 0; i < this->lightList.size(); i++)
    {
        lightListCopy.push_back(lightList[i]);
        if(lightList[i].getPosition()[3] != 0.0f)
            lightListCopy.back().setPosition(glm::vec3(transformation.top() * lightList[i].getPosition()));
        else
            lightListCopy.back().setDirection(glm::vec3(transformation.top() * lightList[i].getPosition()));
    }
    for(i = 0; i < children.size(); i++)
    {
        lightListCopy = children[i]->getLights(lightListCopy, transformation);
    }
    transformation.pop();
    return lightListCopy;
}
