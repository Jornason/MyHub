#ifndef OBJECTXMLREADER_H
#define OBJECTXMLREADER_H
#include <QString>
#include <qxml.h>
#include <GL/gl.h>
#include <vector>
using namespace std;
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Object.h"
#include "Plane.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Box.h"
#include "Cone.h"

class ObjectXMLReader
{
public:
	ObjectXMLReader(){}
	~ObjectXMLReader(){}
    static vector<Object *> readObjectXMLFile(string filename)
	{
		ObjectXMLFileHandler handler;
		QXmlSimpleReader reader;
        QFile xmlFile(QString::fromStdString(filename));
		QXmlInputSource source(&xmlFile);
		vector<Object *> list;
	    reader.setContentHandler(&handler);
	    bool answer = reader.parse(source);
	    if (answer)
	    {
	    	list = handler.getObjectList();
	    }
	    else
            printf("Parsing unsuccessful because %s\n",handler.errorString().toLatin1().constData());
	    return list;
	}

	class ObjectXMLFileHandler: public QXmlDefaultHandler
	{
		public:
		    bool startDocument()
		    {
                obj = NULL;
	   	        return true;
		    }

		    bool characters(const QString& text)
		    {
		    	int c;
		    	float f;
			    QString info;
		        info = text;
                info.remove(QRegExp("[\n\t\r]*"));
                QStringList list = info.split(' ',QString::SkipEmptyParts);
                for (int i=0;i<list.length();i++,index++)
                {
                    c=sscanf_s(list.at(i).toLatin1().constData(),"%f",&f);
                    printf("Read number: %f\n",f);
                    data.push_back(f);
                    if (c!=1)
                        return false;
                }
                return true;
		    }

            bool startElement( const QString & namespaceURI,const QString & localName,const QString & qName,const QXmlAttributes & atts)
		    {
                printf("Start:%s\n",qName.toLatin1().constData());
                if (qName.compare("plane")==0)
		    	{
                    string name;
                    int i=0;
                    bool found = false;
                    while ((i<atts.count()) && (!found))
                    {
                        if (atts.qName(i).compare("name")==0)
                        {
                            name = atts.value(i).toLatin1().constData();
                            found = true;
                        }
                        i++;
                    }
                    obj = new Plane(name);
		    	}
		    	else if (qName.compare("sphere")==0)
		    	{
                    string name;
                    int i=0;
                    bool found = false;
                    while ((i<atts.count()) && (!found))
                    {
                        if (atts.qName(i).compare("name")==0)
                        {
                            name = atts.value(i).toLatin1().constData();
                            found = true;
                        }
                        i++;
                    }
                    obj = new Sphere(name);
                }
		    	else if (qName.compare("cylinder")==0)
		    	{

                    QString inner;
                    float inner_radius;
                    int i=0;
                    bool found = false;
                    while ((i<atts.count()) && (!found))
                    {
                        if (atts.qName(i).compare("inner")==0)
                        {
                                inner = atts.value(i);
                                found = true;
                        }
                        i++;
                    }
                    if (found)
                    {
                        inner_radius = inner.toFloat();
                    }
                    else
                        inner_radius = 0.0f;
                    string name;
                    i=0;
                    found = false;
                    while ((i<atts.count()) && (!found))
                    {
                        if (atts.qName(i).compare("name")==0)
                        {
                            name = atts.value(i).toLatin1().constData();
                            found = true;
                        }
                        i++;
                    }
                    obj = new Cylinder(name,inner_radius);
		    	}
		    	else if (qName.compare("cone")==0)
		    	{
                    string name;
                    int i=0;
                    bool found = false;
                    while ((i<atts.count()) && (!found))
                    {
                        if (atts.qName(i).compare("name")==0)
                        {
                            name = atts.value(i).toLatin1().constData();
                            found = true;
                        }
                        i++;
                    }
                    obj = new Cone(name);
		    	}
                else if (qName.compare("box")==0)
                {
                    string name;
                    int i=0;
                    bool found = false;
                    while ((i<atts.count()) && (!found))
                    {
                        if (atts.qName(i).compare("name")==0)
                        {
                            name = atts.value(i).toLatin1().constData();
                            found = true;
                        }
                        i++;
                    }
                    obj = new Box(name);
                }
                else if (qName.compare("light")==0)
		    	{
		    	}
		    	else if (qName.compare("mesh")==0)
                {
                }
		    	else if (qName.compare("image")==0)
                {
                }
		    	else if (qName.compare("texture")==0)
		    	{		    		
		    	}
		    	return true;
		    }

		    bool endElement( const QString&, const QString&, const QString& qName)
		    {
		    	if (qName.compare("plane")==0)
		    	{
                    list.push_back(obj);
                    obj = NULL;
		    	}
		    	else if (qName.compare("sphere")==0)
		    	{
                    list.push_back(obj);
                    obj = NULL;
                }
		    	else if (qName.compare("cylinder")==0)
		    	{
                    list.push_back(obj);
                    obj = NULL;
		    	}
		    	else if (qName.compare("cone")==0)
		    	{
                    list.push_back(obj);
                    obj = NULL;
		    	}
                else if (qName.compare("box")==0)
                {
                    list.push_back(obj);
                    obj = NULL;
                }
                else if (qName.compare("mesh")==0)
		    	{
		    	}
		    	else if (qName.compare("image")==0)
		    	{
		    	}
		    	else if (qName.compare("texture")==0)
		    	{
		    	}
		    	else if (qName.compare("transform")==0)
		    	{
                    if (obj!=NULL)
                            obj->setTransform(m);
                    m = glm::mat4(1.0f);
		    	}
		    	else if (qName.compare("translate")==0)
		    	{
                    if (data.size()!=3)
                        return false;
                    m = glm::translate(glm::mat4(1.0f),glm::vec3(data[0],data[1],data[2])) * m;
                    data.clear();
		    	}
		    	else if (qName.compare("rotate")==0)
                {
                    if (data.size()!=4)
                        return false;
                    m = glm::rotate(glm::mat4(1.0f),data[0],glm::vec3(data[1],data[2],data[3])) * m;
                    data.clear();
                }
		    	else if (qName.compare("scale")==0)
		    	{
                    if (data.size()!=3)
                        return false;
                    m = glm::scale(glm::mat4(1.0f),glm::vec3(data[0],data[1],data[2])) * m;
                    data.clear();
		    	}
		    	else if (qName.compare("color")==0)
		    	{
                    if (data.size()!=3)
                        return false;
                    if (obj!=NULL)
                        obj->setColor(data[0],data[1],data[2]);
                    data.clear();
		    	}
                else if (qName.compare("ambient")==0)
		    	{
		    	}
		    	else if (qName.compare("diffuse")==0)
		    	{
		    	}
		    	else if (qName.compare("specular")==0)
		    	{	
		    	}
		    	else if (qName.compare("shininess")==0)
		    	{	
		    	}
		    	else if (qName.compare("position")==0)
		    	{
                    if (data.size()!=3)
                        return false;
                    data.clear();
		    	}
		    	else if (qName.compare("direction")==0)
		    	{
                    if (data.size()!=3)
                        return false;
                    data.clear();
		    	}
		    	else if (qName.compare("spotdirection")==0)
		    	{
                    if (data.size()!=3)
                        return false;
                    data.clear();
		    	}
		    	else if (qName.compare("spotangle")==0)
		    	{
                    if (data.size()!=1)
                        return false;
                    data.clear();
		    	}
		    	else if (qName.compare("light")==0)
		    	{
		    	}
		    	else if (qName.compare("material")==0)
		    	{
		    	}
		    	else if (qName.compare("camera")==0)
		    	{
		    	}
		    	else if (qName.compare("eye")==0)
		    	{
		    	}
		    	else if (qName.compare("look")==0)
		    	{
		    	}
                printf("End:%s \n",qName.toLatin1().constData());
		    	return true;
		    }

		    vector<Object *> getObjectList()
		    {
		    	return list;
		    }
		private:
		    vector<float> data;
		    int index;
            Object *obj;
            glm::mat4 m;
		    vector<Object *>list;
		};
};
#endif
