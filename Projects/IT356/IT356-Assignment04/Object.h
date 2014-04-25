#ifndef OBJECT_H_
#define OBJECT_H_

#include "scenegraph.h"
#include "Material.h"
using namespace std;

#define BUFFER_OFFSET(offset) ((void *)(offset))

//Vertex Attributes class
class VertexAttribs
{
public:
    float position[4];
    float normal[4];
    float texcoords[2];
};

/*
 * Object class
 * This class extends the node class and defines an object which will be drawn
 * The object can be animated
 */

class Object : public Node
{
protected:
    enum Buffer_IDs {VertexBuffer,IndexBuffer,NumBuffers};
    enum Attrib_IDs {vPosition=0,vNormal=1,vTexCoord=2};
protected:
    //Vertex array object
    GLuint VAO;
    //Buffers
    GLuint buffers[NumBuffers];
    //vector of vertex attributes
    vector<VertexAttribs> vertexData;
    //vector of triangle indices
    vector<GLuint> triangleIndices;
    //the color of the object
    //glm::vec3 color;
    //the material of the object
    Material material;

private:
    //The animation applied to the object
    Animator* animation;
public:
    GLuint textureID, textureMatrixLocation, textureLocation;
    glm::mat4 texture;
    //Constructor with object name and animator provided
    Object(string name="", Animator* animation = new Animator());
    //DESTRUCTOR
    virtual ~Object(void);
    //draws the object by applying its color, getting the transformation matrix, applying animations, applying previous transformations, binding the VAO, drawing the elements, then popping the transformation
    virtual void draw(stack<glm::mat4> &transformation, GLuint program);
    //SETTERS AND GETTERS
    //virtual void setColor(float r,float g,float b);
    virtual void setMaterial(Material material);
    //glm::vec3& getColor();
    Material getMaterial();
    Animator* getAnimation();
    void setAnimation(Animator* animation);
    //getListOfLights
    vector<Light>& getLights(vector<Light>& lightListCopy, stack<glm::mat4>& transformation);

};

#endif
