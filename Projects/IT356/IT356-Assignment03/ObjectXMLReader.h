#ifndef OBJECTXMLREADER_H
#define OBJECTXMLREADER_H
#include <QString>
#include <qxml.h>
#include "Plane.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Box.h"
#include "Cone.h"
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
    static Scenegraph* readObjectXMLFile(string filename, GLint objectColorLocation, GLint modelviewLocation, glm::mat4& modelview);

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
            GLint getObjectColorLocation();
            void setObjectColorLocation(GLint objectColorLocation);
            GLint getModelviewLocation();
            void setModelviewLocation(GLint modelviewLocation);
            glm::mat4& getModelview();
            void setModelview(glm::mat4& modelview);
            //DESTRUCTOR
            ~ObjectXMLFileHandler();
		private:
            //data for each element attribute
		    vector<float> data;
		    int index;
            //The object being added to the scene
            Node *obj;
            //transformation applied to object
            glm::mat4 m;
            //The scene
            Scenegraph* scene;
            //The location of the object's color
            GLint objectColorLocation;
            //The location of the modelview
            GLint modelviewLocation;
            //The modelview matrix
            glm::mat4 modelview;
		};
};
#endif
