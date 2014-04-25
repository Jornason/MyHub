#include "ObjectXMLReader.h"
ObjectXMLReader::~ObjectXMLReader()
{
}
Scenegraph* ObjectXMLReader::readObjectXMLFile(string& filename)
{
  ObjectXMLFileHandler handler;
  QXmlSimpleReader reader;
  QFile xmlFile(QString::fromStdString(filename));
  QXmlInputSource source(&xmlFile);
  reader.setContentHandler(&handler);
  bool answer = reader.parse(source);
  Scenegraph* graph = NULL;
  graph = handler.getScenegraph();
  if (!answer)
  {
    printf("Parsing unsuccessful because %s\n", handler.errorString().toLatin1().constData());
    delete graph;
    graph = NULL;
  }
  return graph;
}
bool ObjectXMLReader::ObjectXMLFileHandler::startDocument()
{
  node = NULL;
  root = NULL;
  whiteTexture = NULL;
  texture = NULL;
  inTexture = false;
  graph = new Scenegraph();
  stackObjects.push(NULL);
  return true;
}
bool ObjectXMLReader::ObjectXMLFileHandler::characters(const QString& text)
{
  int c;
  float f;
  QString info;
  info = text;
  info.remove(QRegExp("[\n\t\r]*"));
  QStringList list = info.split(' ', QString::SkipEmptyParts);
  for (int i = 0; i < list.length(); i++, index++)
  {
    c = sscanf_s(list.at(i).toLatin1().constData(), "%f", &f);
    printf("Read number: %f\n", f);
    data.push_back(f);
    if (c != 1)
      return true;
  }
  return true;
}
bool ObjectXMLReader::ObjectXMLFileHandler::startElement(const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts)
{
  printf("Start:%s\n", qName.toLatin1().constData());
  if (qName.compare("group") == 0)
  {
    string name;
    int i = 0;
    bool found = false;
    while ((i < atts.count()) && (!found))
    {
      if (atts.qName(i).compare("name") == 0)
      {
        name = atts.value(i).toLatin1().constData();
        found = true;
      }
      i++;
    }
    node = new Group(graph, name);
    stackObjects.push(node);
  }
  else if (qName.compare("plane") == 0)
  {
    string name;
    int i = 0;
    bool found = false;
    while ((i < atts.count()) && (!found))
    {
      if (atts.qName(i).compare("name") == 0)
      {
        name = atts.value(i).toLatin1().constData();
        found = true;
      }
      i++;
    }
    stackObjects.push(new Plane(graph, name));
    ((Object*)stackObjects.top())->setTexture(whiteTexture);
  }
  else if (qName.compare("sphere") == 0)
  {
    string name;
    int i = 0;
    bool found = false;
    while ((i < atts.count()) && (!found))
    {
      if (atts.qName(i).compare("name") == 0)
      {
        name = atts.value(i).toLatin1().constData();
        found = true;
      }
      i++;
    }
    stackObjects.push(new Sphere(graph, name));
    ((Object*)stackObjects.top())->setTexture(whiteTexture);
  }
  else if (qName.compare("box") == 0)
  {
    string name;
    int i = 0;
    bool found = false;
    while ((i < atts.count()) && (!found))
    {
      if (atts.qName(i).compare("name") == 0)
      {
        name = atts.value(i).toLatin1().constData();
        found = true;
      }
      i++;
    }
    stackObjects.push(new Box(graph, name));
    ((Object*)stackObjects.top())->setTexture(whiteTexture);
  }
  else if (qName.compare("cylinder") == 0)
  {
    QString inner;
    float inner_radius;
    int i = 0;
    bool found = false;
    while ((i < atts.count()) && (!found))
    {
      if (atts.qName(i).compare("inner") == 0)
      {
        inner = atts.value(i);
        found = true;
      }
      i++;
    }
    if (found)
      inner_radius = inner.toFloat();
    else
      inner_radius = 0.0f;
    string name;
    i = 0;
    found = false;
    while ((i < atts.count()) && (!found))
    {
      if (atts.qName(i).compare("name") == 0)
      {
        name = atts.value(i).toLatin1().constData();
        found = true;
      }
      i++;
    }
    stackObjects.push(new Cylinder(graph, name, inner_radius));
    ((Object*)stackObjects.top())->setTexture(whiteTexture);
  }
  else if (qName.compare("cone") == 0)
  {
    string name;
    int i = 0;
    bool found = false;
    while ((i < atts.count()) && (!found))
    {
      if (atts.qName(i).compare("name") == 0)
      {
        name = atts.value(i).toLatin1().constData();
        found = true;
      }
      i++;
    }
    stackObjects.push(new Cone(graph, name));
    ((Object*)stackObjects.top())->setTexture(whiteTexture);
  }
  else if (qName.compare("light") == 0)
    l = new Light();
  else if (qName.compare("mesh") == 0)
  {
  }
  else if (qName.compare("image") == 0)
  {
    QString path, name;
    int i = 0;
    bool found = false;
    while ((i < atts.count()) && (!found))
    {
      if (atts.qName(i).compare("path") == 0)
      {
        path = atts.value(i);
        found = true;
      }
      i++;
    }
    if (!found)
      return false;
    i = 0;
    found = false;
    while ((i < atts.count()) && (!found))
    {
      if (atts.qName(i).compare("name") == 0)
      {
        name = atts.value(i);
        found = true;
      }
      i++;
    }
    if (!found)
      return false;
    texture = new Texture();
    if (!texture->createImage(path.toStdString().c_str()))
      return false;
    texture->setName(name.toStdString());
    if (name.compare("white") == 0)
      whiteTexture = texture;
    printf("Read in an image %s for texture purposes\n", path.toStdString().c_str());
  }
  else if (qName.compare("texture") == 0)
  {
    QString name;
    int i;
    bool found = false;
    i = 0;
    while ((i < atts.count()) && (!found))
    {
      if (atts.qName(i).compare("name") == 0)
      {
        name = atts.value(i);
        found = true;
      }
      i++;
    }
    if (!found)
      return false;
    unsigned int j = 0;
    while ((j < textures.size()) && (name.toStdString().compare(textures[j]->getName()) != 0))
      j++;
    if (j < textures.size())
    {
      node = stackObjects.top();
      Object* obj = dynamic_cast<Object*>(node);
      if (obj != NULL)
        obj->setTexture(textures[j]);
      inTexture = true;
    }
    else
      return false;
  }
  return true;
}
bool ObjectXMLReader::ObjectXMLFileHandler::endElement(const QString&, const QString&, const QString& qName)
{
  if (qName.compare("group") == 0)
  {
    node = stackObjects.top();
    stackObjects.pop();
    if (stackObjects.top() != NULL)
    {
      Group* temp = dynamic_cast<Group*>(stackObjects.top());
      if (temp != NULL)
        temp->addChild(node);
    }
    else
    {
      root = node;
      graph->makeScenegraph(root);
    }
    node = NULL;
  }
  else if (qName.compare("plane") == 0)
  {
    node = stackObjects.top();
    stackObjects.pop();
    if (stackObjects.top() != NULL)
    {
      Group* temp = dynamic_cast<Group*>(stackObjects.top());
      if (temp != NULL)
        temp->addChild(node);
    }
    node = NULL;
  }
  else if (qName.compare("sphere") == 0)
  {
    node = stackObjects.top();
    stackObjects.pop();
    if (stackObjects.top() != NULL)
    {
      Group* temp = dynamic_cast<Group*>(stackObjects.top());
      if (temp != NULL)
        temp->addChild(node);
    }
    node = NULL;
  }
  else if (qName.compare("cylinder") == 0)
  {
    node = stackObjects.top();
    stackObjects.pop();
    if (stackObjects.top() != NULL)
    {
      Group* temp = dynamic_cast<Group*>(stackObjects.top());
      if (temp != NULL)
        temp->addChild(node);
    }
    node = NULL;
  }
  else if (qName.compare("cone") == 0)
  {
    node = stackObjects.top();
    stackObjects.pop();
    if (stackObjects.top() != NULL)
    {
      Group* temp = dynamic_cast<Group*>(stackObjects.top());
      if (temp != NULL)
        temp->addChild(node);
    }
    node = NULL;
  }
  else if (qName.compare("box") == 0)
  {
    node = stackObjects.top();
    stackObjects.pop();
    if (stackObjects.top() != NULL)
    {
      Group* temp = dynamic_cast<Group*>(stackObjects.top());
      if (temp != NULL)
        temp->addChild(node);
    }
    node = NULL;
  }
  else if (qName.compare("mesh") == 0)
  {
  }
  else if (qName.compare("image") == 0)
  {
    textures.push_back(texture);
    graph->addTexture(texture);
    texture = NULL;
  }
  else if (qName.compare("texture") == 0)
    inTexture = false;
  else if (qName.compare("transform") == 0)
  {
    node = stackObjects.top();
    if (inTexture)
    {
      Object* obj = dynamic_cast<Object*>(node);
      if (obj != NULL)
        obj->setTextureTransform(m);
    }
    else
      node->setTransform(m);
    m = glm::mat4(1.0f);
  }
  else if (qName.compare("translate") == 0)
  {
    if (data.size() != 3)
      return false;
    m = glm::translate(glm::mat4(1.0f), glm::vec3(data[0], data[1], data[2])) * m;
    data.clear();
  }
  else if (qName.compare("rotate") == 0)
  {
    if (data.size() != 4)
      return false;
    m = glm::rotate(glm::mat4(1.0f), data[0], glm::vec3(data[1], data[2], data[3])) * m;
    data.clear();
  }
  else if (qName.compare("scale") == 0)
  {
    if (data.size() != 3)
      return false;
    m = glm::scale(glm::mat4(1.0f), glm::vec3(data[0], data[1], data[2])) * m;
    data.clear();
  }
  else if (qName.compare("ambient") == 0)
  {
    if (data.size() != 3)
      return false;
    if (l != NULL)
      l->setAmbient(data[0], data[1], data[2]);
    else
      material.setAmbient(data[0], data[1], data[2]);
    data.clear();
  }
  else if (qName.compare("diffuse") == 0)
  {
    if (data.size() != 3)
      return false;
    if (l != NULL)
      l->setDiffuse(data[0], data[1], data[2]);
    else
      material.setDiffuse(data[0], data[1], data[2]);
    data.clear();
  }
  else if (qName.compare("specular") == 0)
  {
    if (data.size() != 3)
      return false;
    if (l != NULL)
      l->setSpecular(data[0], data[1], data[2]);
    else
      material.setSpecular(data[0], data[1], data[2]);
    data.clear();
  }
  else if (qName.compare("shininess") == 0)
  {
    material.setShininess(data[0]);
    data.clear();
  }
  else if (qName.compare("absorption") == 0)
  {
    if (data.size() != 1)
      return false;
    material.setAbsorption(data[0]);
    data.clear();
  }
  else if (qName.compare("reflection") == 0)
  {
    if (data.size() != 1)
      return false;
    material.setReflection(data[0]);
    data.clear();
  }
  else if (qName.compare("transparency") == 0)
  {
    if (data.size() != 1)
      return false;
    material.setTransparency(data[0]);
    data.clear();
  }
  else if (qName.compare("refractive") == 0)
  {
    if (data.size() != 1)
      return false;
    material.setRefractiveIndex(data[0]);
    data.clear();
  }
  else if (qName.compare("position") == 0)
  {
    if (data.size() != 3)
      return false;
    if (l != NULL)
      l->setPosition(glm::vec4(data[0], data[1], data[2], 1.0f));
    data.clear();
  }
  else if (qName.compare("direction") == 0)
  {
    if (data.size() != 3)
      return false;
    if (l != NULL)
      l->setPosition(glm::vec4(data[0], data[1], data[2], 0.0f));
    data.clear();
  }
  else if (qName.compare("spotdirection") == 0)
  {
    if (data.size() != 3)
      return false;
    if (l != NULL)
      l->setSpotDirection(glm::vec4(data[0], data[1], data[2], 0));
    data.clear();
  }
  else if (qName.compare("spotangle") == 0)
  {
    if (data.size() != 1)
      return false;
    if (l != NULL)
      l->setSpotAngle(data[0]);
    data.clear();
  }
  else if (qName.compare("light") == 0)
  {
    node = stackObjects.top();
    if (l != NULL)
    {
      node->addLight(*l);
      delete l;
    }
    l = NULL;
  }
  else if (qName.compare("material") == 0)
  {
    node = stackObjects.top();
    Object* obj = dynamic_cast<Object*>(node);
    if (obj != NULL)
      obj->setMaterial(material);
  }
  printf("End:%s \n", qName.toLatin1().constData());
  return true;
}
Scenegraph* ObjectXMLReader::ObjectXMLFileHandler::getScenegraph()
{
  return graph;
}
