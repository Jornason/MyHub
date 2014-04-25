#include "ObjectXMLReader.h"


ObjectXMLReader::ObjectXMLReader()
{
}

ObjectXMLReader::~ObjectXMLReader()
{
}

Scenegraph* ObjectXMLReader::readObjectXMLFile(string filename, GLint modelviewLocation, glm::mat4& modelview, GLint normalMatrixLocation, GLint mat_ambientLocation, GLint mat_diffuseLocation, GLint mat_specularLocation, GLint mat_shininessLocation, map<string, GLuint>& textureMap, GLint textureLocation, GLint textureMatrixLocation)
{
    //Create a file handler and tell it were the color and modelview
    ObjectXMLFileHandler handler;
    //handler.setObjectColorLocation(objectColorLocation);
    handler.setModelviewLocation(modelviewLocation);
    handler.textMap = textureMap;
    handler.textureLocation = textureLocation;
    handler.textureMatrixLocation = textureMatrixLocation;

    //pass the modelview matrix to the handler (the camera position)
    handler.setModelview(modelview);

    handler.setNormalMatrixLocation(normalMatrixLocation);
    handler.setmatAmbientLocation(mat_ambientLocation);
    handler.setmatDiffuseLocation(mat_diffuseLocation);
    handler.setmatShininessLocation(mat_shininessLocation);
    handler.setmatSpecularLocation(mat_specularLocation);

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
    //scene = new Scenegraph(objectColorLocation, modelviewLocation, modelview);
    scene = new Scenegraph(modelviewLocation, modelview, normalMatrixLocation, mat_ambientLocation, mat_diffuseLocation, mat_shininessLocation, mat_specularLocation);
    //instantiate the object
    scene->textureLocation = textureLocation;
    scene->textureMatrixLocation = textureMatrixLocation;
    obj = NULL;
    material.init();
    //set the scene's root
    scene->setRoot(new Group("base"));
    //set the scene's current group
    scene->getCurrentGroup().push(scene->getRoot());
    inTexture = false;
    inLight = false;
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
        //printf("Read number: %f\n",f);
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
        inLight = true;
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
        light = new Light();
        light->init();
        light->setScene(scene);
    }
    else if (qName.compare("mesh")==0)
    {
      QString file;
      string filename;
      int i=0;
      bool found = false;
      while ((i<atts.count()) && (!found))
      {
          if (atts.qName(i).compare("filename")==0)
          {
                  file = atts.value(i);
                  found = true;
          }
          i++;
      }
      if (found)
      {
          filename = file.toStdString();
      }
      else
          filename = "";
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
      obj = new Mesh(name,filename);
      obj->setScene(scene);
    }
    else if (qName.compare("image")==0)
    {
        //In the case of the cylinder, handle the inner radius
        QString inner;
        string path;
        int i=0;
        bool found = false;
        while ((i<atts.count()) && (!found))
        {
            if (atts.qName(i).compare("path")==0)
            {
                    inner = atts.value(i);
                    found = true;
            }
            i++;
        }
        if (found)
        {
            path = inner.toLatin1();
        }
        else
            path = "";
        nameofTexture;
        i=0;
        found = false;
        while ((i<atts.count()) && (!found))
        {
            if (atts.qName(i).compare("name")==0)
            {
                inner = atts.value(i);
                found = true;
            }
            i++;
        }
        if (found)
        {
            nameofTexture = inner.toLatin1();
        }
        else
            nameofTexture = "";
        found = false;
        FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path.c_str());//Automatocally detects the format(from over 20 formats!)
        FIBITMAP* image = FreeImage_Load(format, path.c_str());

        FIBITMAP* temp = image;
        image = FreeImage_ConvertTo32Bits(image);
        FreeImage_Unload(temp);

        int w = FreeImage_GetWidth(image);
        int h = FreeImage_GetHeight(image);
        cout<<"The size of the image is: "<<w<<"*"<<h<<endl; //Some debugging code


        char* pixels = (char*)FreeImage_GetBits(image);
        //FreeImage loads in BGRA format, so you need to swap some bytes(Or use GL_BGRA).


        glGenTextures(1,&textID); //get a unique texture ID
        glBindTexture(GL_TEXTURE_2D,textID);// bind this texture as "current". All texture commands henceforth will apply to this ID

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);

        glGenerateMipmap(GL_TEXTURE_2D);

        cout << textID << endl;
        glBindTexture(GL_TEXTURE_2D,0); //unbind
        std::map<string,GLuint>::iterator it = scene->textureMap.end();
        string name = nameofTexture;

        scene->textureMap.insert(it, make_pair(name, textID));


    }
    else if (qName.compare("texture")==0)
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
        if(obj!=NULL)
        {
           GLuint ID = scene->textureMap[name];
           dynamic_cast<Object*>(obj)->textureID = ID;
           inTexture = true;
        }

    }
    return true;
}

bool ObjectXMLReader::ObjectXMLFileHandler::endElement( const QString&, const QString&, const QString& qName)
{
    //For the end of each element, add it to the current group and nullify the object
    //cout<<qName.toStdString()<<endl;
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
        scene->getCurrentGroup().top()->addChild(obj);
        obj = NULL;
    }
    else if (qName.compare("image")==0)
    {

    }
    else if (qName.compare("texture")==0)
    {
        inTexture = false;

    }
    //set the transform
    else if (qName.compare("transform")==0)
    {

        if (obj!=NULL)
        {
            if(inTexture)
            {
                dynamic_cast<Object*>(obj)->texture = m;
            }
            else
            {
                obj->setTransform(m);
            }
        }
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
        //cout<<"yes"<<endl;
        if (data.size()!=3)
        {
            //cout<<data.size()<<endl;
            //cout<<"1"<<endl;
            return false;
        }
        //cout<<"3"<<endl;
        m = glm::scale(glm::mat4(1.0f),glm::vec3(data[0],data[1],data[2])) * m;
        //cout<<"2"<<endl;
        data.clear();
        //cout<<"yes"<<endl;
    }
    //apply a color to the object
    /*else if (qName.compare("color")==0)
    {
        if (data.size()!=3)
            return false;
        if (obj!=NULL)
            dynamic_cast<Object*>(obj)->setColor(data[0],data[1],data[2]);
        data.clear();
    }*/
    else if (qName.compare("ambient")==0)
    {
        if (data.size()!=3)
            return false;


        if (inLight)
            light->setAmbient(data[0], data[1], data[2]);
        else
            material.setAmbient(data[0],data[1],data[2]);
        data.clear();
    }
    else if (qName.compare("diffuse")==0)
    {
        if (data.size()!=3)
            return false;


        if (inLight)
            light->setDiffuse(data[0], data[1], data[2]);
        else
            material.setDiffuse(data[0],data[1],data[2]);
        data.clear();
    }
    else if (qName.compare("specular")==0)
    {
        if (data.size()!=3)
            return false;


        if (inLight)
            light->setSpecular(data[0], data[1], data[2]);
        else
            material.setSpecular(data[0],data[1],data[2]);
        data.clear();
    }
    else if (qName.compare("shininess")==0)
    {
        if (data.size()!=1)
            return false;
        material.setShininess(data[0]);
        data.clear();
    }
    else if (qName.compare("position")==0)
    {
        if (data.size()!=3)
            return false;
        if (inLight)
            light->setPosition(data[0], data[1], data[2]);
        data.clear();
    }
    else if (qName.compare("direction")==0)
    {
        if (data.size()!=3)
            return false;
        if (inLight)
            light->setDirection(data[0], data[1], data[2]);
        data.clear();
    }
    else if (qName.compare("spotdirection")==0)
    {
        if (data.size()!=3)
            return false;
        if (inLight)
            light->setSpotDirection(data[0], data[1], data[2]);
        data.clear();
    }
    else if (qName.compare("spotangle")==0)
    {
        if (data.size()!=1)
            return false;
        if (inLight)
            light->setSpotAngle(data[0]);
        data.clear();
    }
    else if (qName.compare("light")==0)
    {
        if (obj!=NULL)
        {
                Light newLight = *light;
                obj->addLight(newLight);
                cout<<"light added"<<endl;
        }
        inLight = false;
        light = NULL;
    }
    else if (qName.compare("material")==0)
    {
        if (obj!=NULL)
                dynamic_cast<Object*>(obj)->setMaterial(material);
        material.init();
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

/*GLint ObjectXMLReader::ObjectXMLFileHandler::getObjectColorLocation()
{
    return objectColorLocation;
}

void ObjectXMLReader::ObjectXMLFileHandler::setObjectColorLocation(GLint objectColorLocation)
{
    this->objectColorLocation = objectColorLocation;
}*/

GLint ObjectXMLReader::ObjectXMLFileHandler::getNormalMatrixLocation()
{
    return normalMatrixLocation;
}

void ObjectXMLReader::ObjectXMLFileHandler::setNormalMatrixLocation(GLint normalMatrixLocation)
{
    this->normalMatrixLocation = normalMatrixLocation;
}
GLint ObjectXMLReader::ObjectXMLFileHandler::getmatAmbientLocation()
{
    return mat_ambientLocation;
}
void ObjectXMLReader::ObjectXMLFileHandler::setmatAmbientLocation(GLint matAmbientLocation)
{
    this->mat_ambientLocation = matAmbientLocation;
}
GLint ObjectXMLReader::ObjectXMLFileHandler::getmatDiffuseLocation()
{
    return mat_diffuseLocation;
}
void ObjectXMLReader::ObjectXMLFileHandler::setmatDiffuseLocation(GLint matDiffuseLocation)
{
    this->mat_diffuseLocation = matDiffuseLocation;
}
GLint ObjectXMLReader::ObjectXMLFileHandler::getmatSpecularLocation()
{
    return mat_specularLocation;
}
void ObjectXMLReader::ObjectXMLFileHandler::setmatSpecularLocation(GLint matSpecularLocation)
{
    this->mat_specularLocation = matSpecularLocation;
}
GLint ObjectXMLReader::ObjectXMLFileHandler::getmatShininessLocation()
{
    return mat_shininessLocation;
}
void ObjectXMLReader::ObjectXMLFileHandler::setmatShininessLocation(GLint matShininessLocation)
{
    this->mat_shininessLocation=matShininessLocation;
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
