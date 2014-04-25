#ifndef OBJECTXMLREADER_H
#define OBJECTXMLREADER_H
#include <QString>
#include "FreeImage.h"
#include <qxml.h>
#include "Plane.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Box.h"
#include "Cone.h"
#include "mesh.h"
#include <map>
using namespace std;

/*
 * ObjectXMLReader class
 * This class reads in a txt file and parses the file for scene information
 */

class ObjectXMLReader
{
public:
    //Default constructor
    ObjectXMLReader();
    //DESTRUCTOR
    ~ObjectXMLReader();
    //Read the file and get a scenegraph from it
    static Scenegraph* readObjectXMLFile(string filename, GLint modelviewLocation, glm::mat4& modelview, GLint normalMatrixLocation, GLint mat_ambientLocation, GLint mat_diffuseLocation, GLint mat_specularLocation, GLint mat_shininessLocation, map<string, GLuint>& textureMap, GLint textureLocation, GLint textureMatrixLocation);

    /*
     * ObjectXMLFileHandler class
     * This class extends QXML's defauld handler and overrides certain methods to suit this program
     */

	class ObjectXMLFileHandler: public QXmlDefaultHandler
	{
		public:
            //Called when you encounter the start of the document
            bool startDocument();
            //parse characters
            bool characters(const QString& text);
            //Called when you encounter the start of a scene element
            bool startElement(const QString & namespaceURI,const QString & localName,const QString & qName,const QXmlAttributes & atts);
            //Called when you encounter the end of a scene element
            bool endElement( const QString&, const QString&, const QString& qName);
            //SETTERS AND GETTERS
            Scenegraph* getScene();
            void setScene(Scenegraph* scene);
            //GLint getObjectColorLocation();
            //void setObjectColorLocation(GLint objectColorLocation);
            GLint getModelviewLocation();
            void setModelviewLocation(GLint modelviewLocation);
            GLint getNormalMatrixLocation();
            void setNormalMatrixLocation(GLint normalMatrixLocation);
            GLint getmatAmbientLocation();
            void setmatAmbientLocation(GLint matAmbientLocation);
            GLint getmatDiffuseLocation();
            void setmatDiffuseLocation(GLint matDiffuseLocation);
            GLint getmatSpecularLocation();
            void setmatSpecularLocation(GLint matSpecularLocation);
            GLint getmatShininessLocation();
            void setmatShininessLocation(GLint matShininessLocation);
            glm::mat4& getModelview();
            void setModelview(glm::mat4& modelview);
            map<string, GLuint> textMap;
            //DESTRUCTOR
            ~ObjectXMLFileHandler();
            GLuint textID;
            int lightID;
            string nameofTexture;
            GLint textureLocation, textureMatrixLocation;
		private:
            //data for each element attribute
		    vector<float> data;
		    int index;
            //The object being added to the scene
            Node *obj;
            //transformation applied to object
            glm::mat4 m;
            Material material;
            Light* light;
            //The scene
            Scenegraph* scene;
            //The location of the object's color
            //GLint objectColorLocation;
            //The location of the modelview
            GLint modelviewLocation;
            //The modelview matrix
            glm::mat4 modelview;
            GLint normalMatrixLocation;
            bool inTexture;
            bool inLight;
            GLint mat_ambientLocation,mat_diffuseLocation,mat_specularLocation,mat_shininessLocation;

		};
};
#endif
