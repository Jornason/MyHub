#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H
#include "Node.h"
#include "Texture.h"
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <stack>
#include <map>
using namespace std;
class Scenegraph
{
    friend class Object;
    typedef struct
    {
      GLint positionLocation, ambientLocation, diffuseLocation;
      GLint specularLocation, spotDirectionLocation, spotCutoffLocation;
    } LightLocation;
    typedef struct
    {
      GLint ambientLocation, diffuseLocation, specularLocation;
      GLint shininessLocation;
    } MaterialLocation;
  public:
    Scenegraph();
    ~Scenegraph();
    void makeScenegraph(Node* root);
    void initShaderProgram(GLint shaderProgram);
    void draw(stack<glm::mat4>& modelView);
    void animate(float t);
    void enableLights(stack<glm::mat4>& modelView);
    void addTexture(Texture* texture);
    GLfloat* raytrace(int width, int height, stack<glm::mat4>& modelview);
    bool raycast(Ray& r, stack<glm::mat4>& modelview, glm::vec3& color, int recursiveIndex);
    glm::vec3& shade(HitRecord& h, stack<glm::mat4> modelview, glm::vec2& refractiveIndex, glm::vec3& color);
    Node* root;
    map<string, Node*> nodemap;
    vector<Light> lights;
    GLint modelviewLocation, normalmatrixLocation, textureMatrixLocation;
    GLint numLightsLocation, textureLocation;
    MaterialLocation materialLocations;
    LightLocation lightLocation[10];
    GLuint materialBO;
    vector<Texture*> textures;
};
#endif
