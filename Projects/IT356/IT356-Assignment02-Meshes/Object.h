#ifndef OBJECT_H_
#define OBJECT_H_
#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <string>
using namespace std;
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>
#define BUFFER_OFFSET(offset) ((void *)(offset))

class VertexAttribs
{
public:
    float position[4];
    float normal[4];
    float texcoords[2];
};

class Object
{
protected:
    enum Buffer_IDs {VertexBuffer,IndexBuffer,NumBuffers};
    enum Attrib_IDs {vPosition=0,vNormal=1,vTexCoord=2};
protected:
    GLuint VAO;
    GLuint buffers[NumBuffers];
    vector<VertexAttribs> vertexData;
    vector<GLuint> triangleIndices;
    glm::vec3 color;
    //transformation stack
    stack<glm::mat4> transform;
    string name;
public:
    Object(string name="")
    {
        glGenVertexArrays(1,&VAO);
        glBindVertexArray(VAO);
        glGenBuffers(NumBuffers,buffers);
        setName(name);
	}

	virtual ~Object(void)
	{
        if (VAO!=0)
        {
            glDeleteBuffers(NumBuffers,buffers);
            glDeleteVertexArrays(1,&VAO);
        }
	}

    virtual void draw()
	{
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,triangleIndices.size(),GL_UNSIGNED_INT,BUFFER_OFFSET(0));
	}

    void setTransform(glm::mat4& obj)
	{
        //push the new transformation to the stack
        transform.push(obj);
	}

    glm::mat4 getTransform()
    {
        //return the accumulated transformations
        return transform.top();
    }
    void clearTransform()
    {
        transform.pop();
    }

    void setName(const string& name)
    {
        this->name = name;
    }

    virtual void setColor(float r,float g,float b)
    {
        color = glm::vec3(r,g,b);
    }

    glm::vec3 getColor()
    {
        return color;
    }

};

#endif
