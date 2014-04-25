#include "node.h"
#include <stack>
using namespace std;

Node::Node()
{
    name = "default";
}

Node::Node(string& name, Scenegraph* scene)
{
    this->name = name;
    this->scene = scene;
    this->transform.push(glm::mat4(1.0));
}

Node::~Node()
{

}

void Node::draw(stack<glm::mat4> &transformation)
{
    //Gets the node's transformation, applies it to the stack, and then immediately pops it
    glm::mat4 currentTransformationMatrix = getTransform();
    currentTransformationMatrix = transformation.top() * currentTransformationMatrix;
    transformation.push(currentTransformationMatrix);
    transformation.pop();
}

void Node::setTransform(glm::mat4& obj)
{
    transform.push(obj);
}

glm::mat4& Node::getTransform()
{
    return transform.top();
}

void Node::clearTransform()
{
    transform.pop();
}

void Node::setName(const string& name)
{
    this->name = name;
}

string& Node::getName()
{
    return name;
}

void Node::setScene(Scenegraph* scene)
{
    this->scene = scene;
}

Scenegraph* Node::getScene()
{
    return scene;
}

