#ifndef GROUP_H
#define GROUP_H
#include <vector>
#include "node.h"
using namespace std;

/*
 * Group class
 * This class extends the node class and defines a group of objects
 * The objects, which themselves can be groups, are stored in a vector
 * Animations can be applied to the Group
 * When a group is drawn, it applies its own transformation to the stack and draws the children
 */
class Group : public Node
{
public:
    //Default constructor
    Group(string name="", Scenegraph* scene = NULL, Animator* animation = new Animator());
    //Add a child to the group
    void addChild(Node* child);
    //draw the group by applying a transformation and drawing its children, then pop the transformation
    virtual void draw(stack<glm::mat4> &transformation);
    //SETTERS AND GETTERS
    vector<Node*>& getChildren();
    void setChildren(vector<Node*>& children);
    Animator* getAnimation();
    void setAnimation(Animator* animation);
    //DESTRUCTOR
    ~Group();
private:
    //The vector of children nodes
    vector<Node*> children;
    //The animation to apply to this group
    Animator* animation;

};

#endif // GROUP_H
