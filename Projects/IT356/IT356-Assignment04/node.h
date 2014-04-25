#ifndef NODE_H
#define NODE_H
#include <stack>
#include <glm/glm.hpp>
#include "animator.h"
#include "Light.h"
#include <vector>
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
        virtual void draw(stack<glm::mat4> &transformation, GLuint program);
        //SETTERS AND GETTERS
        void setTransform(glm::mat4& obj);
        glm::mat4& getTransform();
        void clearTransform();
        void setName(const string& name);
        string& getName();
        void setScene(Scenegraph* scene);
        Scenegraph* getScene();
        void setLightList(vector<Light>& lightList);
        vector<Light>& getLightList();
        //add light to list of lights
        void addLight(Light &light);
        //get the light list
        virtual vector<Light>& getLights(vector<Light>& listOfLights, stack<glm::mat4>& transformation);
private:
        //The name of the node
        string name;
        //The node's transformation matrix
        stack<glm::mat4> transform;      
protected:
        //The scenegraph the node is attached to
        Scenegraph* scene;
        //list of lights
        vector<Light> lightList;
};

#endif // NODE_H
