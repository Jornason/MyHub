#ifndef OBJECT_H_
#define OBJECT_H_
#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <string>
#include <cmath>
#include "Node.h"
#include "Scenegraph.h"
#include "Material.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;
#define BUFFER_OFFSET(offset) ((void *)(offset))
class VertexAttribs
{
  public:
    float position[4];
    float normal[4];
    float texcoords[2];
};
class Object: public Node
{
  protected:
    enum Buffer_IDs {VertexBuffer, IndexBuffer, NumBuffers};
    enum Attrib_IDs {vPosition = 0, vNormal = 1, vTexCoord = 2};
    GLuint VAO;
    GLuint buffers[NumBuffers];
    vector<VertexAttribs> vertexData;
    vector<GLuint> triangleIndices;
    Material material;
    Texture* texture;
    glm::mat4 textureTransform;
    int numIndices;
    string name;
  public:
    Object(Scenegraph* graph, string name = "");
    virtual ~Object();
    virtual void getLights(vector<Light>& lightvector, stack<glm::mat4>& modelView);
    virtual void draw(stack<glm::mat4>& modelView);
    virtual void setMaterial(Material& m);
    Material& getMaterial();
    Texture* getTexture();
    void setTexture(Texture* texture);
    void setTextureTransform(glm::mat4& mat);
};
#endif
