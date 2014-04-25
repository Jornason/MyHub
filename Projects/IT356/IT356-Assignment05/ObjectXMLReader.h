#ifndef OBJECTXMLREADER_H
#define OBJECTXMLREADER_H
#include <QString>
#include <qxml.h>
#include "Windows.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <stack>
#include "Node.h"
#include "Object.h"
#include "Group.h"
#include "Plane.h"
#include "Cone.h"
#include "Box.h"
#include "Sphere.h"
#include "Cylinder.h"
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "Material.h"
#include "Texture.h"
#include "Light.h"
#include "Scenegraph.h"
using namespace std;
class ObjectXMLReader
{
  public:
    ObjectXMLReader();
    ~ObjectXMLReader();
    static Scenegraph* readObjectXMLFile(string& filename);
    class ObjectXMLFileHandler: public QXmlDefaultHandler
    {
      public:
        bool startDocument();
        bool characters(const QString& text);
        bool startElement(const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts);
        bool endElement(const QString&, const QString&, const QString& qName);
        Scenegraph* getScenegraph();
      private:
        vector<float> data;
        int index;
        Node* node, *root;
        Scenegraph* graph;
        Material material;
        glm::mat4 m;
        Light* l;
        bool inTexture;
        stack<Node*> stackObjects;
        Texture* whiteTexture, *texture;
        vector<Texture*> textures;
    };
};
#endif
