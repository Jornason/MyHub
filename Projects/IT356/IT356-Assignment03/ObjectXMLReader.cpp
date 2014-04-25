#include "ObjectXMLReader.h"


ObjectXMLReader::ObjectXMLReader()
{

}

ObjectXMLReader::~ObjectXMLReader()
{

}

Scenegraph* ObjectXMLReader::readObjectXMLFile(string filename, GLint objectColorLocation, GLint modelviewLocation, glm::mat4& modelview)
{
    //Create a file handler and tell it were the color and modelview
    ObjectXMLFileHandler handler;
    handler.setObjectColorLocation(objectColorLocation);
    handler.setModelviewLocation(modelviewLocation);
    //pass the modelview matrix to the handler (the camera position)
    handler.setModelview(modelview);
    //create a simple reader
    QXmlSimpleReader reader;
    //create a Qfile
    QFile xmlFile(QString::fromStdString(filename));
    //specify the source
    QXmlInputSource source(&xmlFile);

    //declare a new scenegraph
    Scenegraph* scene;
    //set the content handler
    reader.setContentHandler(&handler);
    //parse
    bool answer = reader.parse(source);
    //if successful, set the scene
    if (answer)
    {
        scene = handler.getScene();
    }
    //otherwise, print an error
    else
        printf("Parsing unsuccessful because %s\n",handler.errorString().toLatin1().constData());
    return scene;
}

bool ObjectXMLReader::ObjectXMLFileHandler::startDocument()
{
    //set the scenegraph
    scene = new Scenegraph(objectColorLocation, modelviewLocation, modelview);
    //instantiate the object
    obj = NULL;
    //set the scene's root
    scene->setRoot(new Group("base"));
    //set the scene's current group
    scene->getCurrentGroup().push(scene->getRoot());
    return true;
}

bool ObjectXMLReader::ObjectXMLFileHandler::characters(const QString& text)
{
    int c;
    float f;
    QString info;
    info = text;
    //remove spaces from the data
    info.remove(QRegExp("[\n\t\r]*"));
    //split the data
    QStringList list = info.split(' ',QString::SkipEmptyParts);
    //for each data item
    for (int i=0;i<list.length();i++,index++)
    {
        //scan the number
        c=sscanf_s(list.at(i).toLatin1().constData(),"%f",&f);
        printf("Read number: %f\n",f);
        //push the number to data
        data.push_back(f);
        if (c!=1)
            return false;
    }
    return true;
}

bool ObjectXMLReader::ObjectXMLFileHandler::startElement(const QString & namespaceURI,const QString & localName,const QString & qName,const QXmlAttributes & atts)
{
    //for each item encountered, collect the name and create the object. Then set its scene
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
        obj->setScene(scene);
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
        obj->setScene(scene);
    }
    else if (qName.compare("cylinder")==0)
    {
        //In the case of the cylinder, handle the inner radius
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
        obj->setScene(scene);
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
        obj->setScene(scene);
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
        obj->setScene(scene);
    }
    else if (qName.compare("group")==0)
    {
        string name;
        int i=0;
        bool found = false;
        while((i<atts.count()) && (!found))
        {
            if(atts.qName(i).compare("name")==0)
            {
                name = atts.value(i).toLatin1().constData();
                found = true;
            }
            i++;
        }
        obj = new Group(name, scene, scene->getAnimation());
        //in the case of groups, push the group the the currentGroup stack
        scene->getCurrentGroup().top()->addChild(obj);
        scene->getCurrentGroup().push((Group*)obj);
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

bool ObjectXMLReader::ObjectXMLFileHandler::endElement( const QString&, const QString&, const QString& qName)
{
    //For the end of each element, add it to the current group and nullify the object
    if (qName.compare("plane")==0)
    {
        scene->getCurrentGroup().top()->addChild(obj);
        obj = NULL;
    }
    else if (qName.compare("sphere")==0)
    {
        scene->getCurrentGroup().top()->addChild(obj);
        obj = NULL;
    }
    else if (qName.compare("cylinder")==0)
    {
        scene->getCurrentGroup().top()->addChild(obj);
        obj = NULL;
    }
    else if (qName.compare("cone")==0)
    {
        scene->getCurrentGroup().top()->addChild(obj);
        obj = NULL;
    }
    else if (qName.compare("box")==0)
    {
        scene->getCurrentGroup().top()->addChild(obj);
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
    //set the transform
    else if (qName.compare("transform")==0)
    {
        if (obj!=NULL)
                obj->setTransform(m);
        m = glm::mat4(1.0f);
    }
    //translate the object
    else if (qName.compare("translate")==0)
    {
        if (data.size()!=3)
            return false;
        m = glm::translate(glm::mat4(1.0f),glm::vec3(data[0],data[1],data[2])) * m;
        data.clear();
    }
    //rotate the object
    else if (qName.compare("rotate")==0)
    {
        if (data.size()!=4)
            return false;
        m = glm::rotate(glm::mat4(1.0f),data[0],glm::vec3(data[1],data[2],data[3])) * m;
        data.clear();
    }
    //scale the object
    else if (qName.compare("scale")==0)
    {
        if (data.size()!=3)
            return false;
        m = glm::scale(glm::mat4(1.0f),glm::vec3(data[0],data[1],data[2])) * m;
        data.clear();
    }
    //apply a color to the object
    else if (qName.compare("color")==0)
    {
        if (data.size()!=3)
            return false;
        if (obj!=NULL)
            dynamic_cast<Object*>(obj)->setColor(data[0],data[1],data[2]);
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
    //at the end of a group, pop the group from the current group stack
    else if (qName.compare("group")==0)
    {
        scene->getCurrentGroup().pop();
        obj = NULL;
    }
    printf("End:%s \n",qName.toLatin1().constData());
    return true;
}

Scenegraph* ObjectXMLReader::ObjectXMLFileHandler::getScene()
{
    return scene;
}

void ObjectXMLReader::ObjectXMLFileHandler::setScene(Scenegraph* scene)
{
    this->scene = scene;
}

GLint ObjectXMLReader::ObjectXMLFileHandler::getObjectColorLocation()
{
    return objectColorLocation;
}

void ObjectXMLReader::ObjectXMLFileHandler::setObjectColorLocation(GLint objectColorLocation)
{
    this->objectColorLocation = objectColorLocation;
}

GLint ObjectXMLReader::ObjectXMLFileHandler::getModelviewLocation()
{
    return modelviewLocation;
}

void ObjectXMLReader::ObjectXMLFileHandler::setModelviewLocation(GLint modelviewLocation)
{
    this->modelviewLocation = modelviewLocation;
}

glm::mat4& ObjectXMLReader::ObjectXMLFileHandler::getModelview()
{
    return modelview;
}

void ObjectXMLReader::ObjectXMLFileHandler::setModelview(glm::mat4& modelview)
{
    this->modelview = modelview;
}

ObjectXMLReader::ObjectXMLFileHandler::~ObjectXMLFileHandler()
{
    delete obj;
}
