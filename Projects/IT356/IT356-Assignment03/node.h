#ifndef NODE_H
#define NODE_H
#include <stack>
#include <glm/glm.hpp>
#include "animator.h"
using namespace std;

//Declare a scenegraph class
class Scenegraph;

/*
 * Node class
 * This class defines a node in the scenegraph
 * It defines a draw method that should always be overridden (it doesn't actually do anything)
 * It keeps track of the scenegraph it is attached to
 */

class Node
{
public:
        //Default constructor
        Node();
        //Constructor with name of node and scene it is attached to provided
        Node(string& name, Scenegraph* scene);
        //DESTRUCTOR
        virtual ~Node();
        //Draw method (should always be overridden, doesn't actually do anything)
        virtual void draw(stack<glm::mat4> &transformation);
        //SETTERS AND GETTERS
        void setTransform(glm::mat4& obj);
        glm::mat4& getTransform();
        void clearTransform();
        void setName(const string& name);
        string& getName();
        void setScene(Scenegraph* scene);
        Scenegraph* getScene();
private:
        //The name of the node
        string name;
        //The node's transformation matrix
        stack<glm::mat4> transform;
protected:
        //The scenegraph the node is attached to
        Scenegraph* scene;
};

#endif // NODE_H
