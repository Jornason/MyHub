#include "scenegraph.h"

Scenegraph::Scenegraph(GLint modelviewLocation, glm::mat4& modelview, GLint normalMatrixLocation, GLint mat_ambientLocation, GLint mat_diffuseLocation, GLint mat_shininessLocation, GLint mat_specularLocation)
{
    //this->objectColorLocation = objectColorLocation;
    this->modelViewLocation = modelviewLocation;
    this->modelview = modelview;
    this->animation = new Animator();
    this->normalMatrixLocation=normalMatrixLocation;
    this->mat_ambientLocation=mat_ambientLocation;
    this->mat_diffuseLocation=mat_diffuseLocation;
    this->mat_shininessLocation=mat_shininessLocation;
    this->mat_specularLocation=mat_specularLocation;
}

void Scenegraph::draw(stack<glm::mat4> &transformationstack, GLuint program)
{
    //get the latest transformation from the stack.
    glm::mat4 latesttransformation = transformationstack.top();
    //clean out the stack of transformations
    while(!transformationstack.empty())
    {
        transformationstack.pop();
    }
    //modify the transformation with the modelview and push it to the stack
    transformationstack.push(modelview * latesttransformation);
    //vector<Light*> lightList = getLights();
    //draw the scene
    root->draw(transformationstack, program);
}

void Scenegraph::increaseAngle()
{
    //call the animator's increasAngle() function
    this->animation->increaseAngle();
}

Scenegraph::~Scenegraph()
{
    delete animation;
    delete root;
}

Group* Scenegraph::getRoot()
{
    return root;
}

void Scenegraph::setRoot(Group* root)
{
    this->root = root;
}

/*GLint Scenegraph::getObjectColorLocation()
{
    return objectColorLocation;
}

void Scenegraph::setObjectColorLocation(GLint objectColorLocation)
{
    this->objectColorLocation = objectColorLocation;
}*/

GLint Scenegraph::getModelviewLocation()
{
    return modelViewLocation;
}

void Scenegraph::setModelviewLocation(GLint modelviewLocation)
{
    this->modelViewLocation = modelviewLocation;
}

glm::mat4& Scenegraph::getModelview()
{
    return modelview;
}

void Scenegraph::setModelview(glm::mat4& modelview)
{
    this->modelview = modelview;
}

stack<Group*>& Scenegraph::getCurrentGroup()
{
    return currentGroup;
}

void Scenegraph::setCurrentGroup(stack<Group*>& currentGroup)
{
    this->currentGroup = currentGroup;
}

Animator* Scenegraph::getAnimation()
{
    return animation;
}

void Scenegraph::setAnimation(Animator* animation)
{
    this->animation = animation;
}

vector<Light> Scenegraph::getLights(stack<glm::mat4> &transformationstack)
{
    //get the latest transformation from the stack.
    glm::mat4 latesttransformation = transformationstack.top();
    //clean out the stack of transformations
    while(!transformationstack.empty())
    {
        transformationstack.pop();
    }
    //modify the transformation with the modelview and push it to the stack
    transformationstack.push(modelview * latesttransformation);
    vector<Light> lightListCopy;
    lightListCopy = this->root->getLights(lightListCopy, transformationstack);
    return lightListCopy;
}

void Scenegraph::enableLights(vector<Light>& lightList, GLuint program)
{
    cout<<"enable lights"<<endl;
    unsigned int i = 0;
    //cout<<"size = "<<lightlocation.size()<<endl;
    lightlocation.clear();
    for(i = 0; i < lightList.size(); i++)
    {
        stringstream name;

        name << "light[" << i << "].ambient";
        LightLocation l = {0, 0, 0, 0};
        lightlocation.push_back(l);

        lightlocation[i].ambientLocation = glGetUniformLocation(program,name.str().c_str());

        name.clear();//clear any bits set
        name.str(std::string());

        name << "light[" << i << "].diffuse";

        lightlocation[i].diffuseLocation = glGetUniformLocation(program,name.str().c_str());

        name.clear();//clear any bits set
        name.str(std::string());

        name << "light[" << i << "].specular";

        lightlocation[i].specularLocation = glGetUniformLocation(program,name.str().c_str());

        name.clear();//clear any bits set
        name.str(std::string());

        name << "light[" << i << "].position";

        lightlocation[i].positionLocation = glGetUniformLocation(program,name.str().c_str());

        name.clear();//clear any bits set
        name.str(std::string());
    }
    glUniform1i(numLightsLocation,lightList.size());

    //light properties
    for (i=0;i<lightList.size();i++)
    {
        glUniform3fv(lightlocation[i].ambientLocation,1,glm::value_ptr(lightList[i].getAmbient()));
        glUniform3fv(lightlocation[i].diffuseLocation,1,glm::value_ptr(lightList[i].getDiffuse()));
        glUniform3fv(lightlocation[i].specularLocation,1,glm::value_ptr(lightList[i].getSpecular()));
        glUniform4fv(lightlocation[i].positionLocation,1,glm::value_ptr(lightList[i].getPosition()));
    }
    cout<<"finish enable lights"<<endl;

}

GLint Scenegraph::getNormalMatrixLocation()
{
    return normalMatrixLocation;
}

void Scenegraph::setNormalMatrixLocation(GLint normalMatrixLocation)
{
    this->normalMatrixLocation = normalMatrixLocation;
}
GLint Scenegraph::getmatAmbientLocation()
{
    return mat_ambientLocation;
}
void Scenegraph::setmatAmbientLocation(GLint matAmbientLocation)
{
    this->mat_ambientLocation = matAmbientLocation;
}
GLint Scenegraph::getmatDiffuseLocation()
{
    return mat_diffuseLocation;
}
void Scenegraph::setmatDiffuseLocation(GLint matDiffuseLocation)
{
    this->mat_diffuseLocation = matDiffuseLocation;
}
GLint Scenegraph::getmatSpecularLocation()
{
    return mat_specularLocation;
}
void Scenegraph::setmatSpecularLocation(GLint matSpecularLocation)
{
    this->mat_specularLocation = matSpecularLocation;
}
GLint Scenegraph::getmatShininessLocation()
{
    return mat_shininessLocation;
}
void Scenegraph::setmatShininessLocation(GLint matShininessLocation)
{
    this->mat_shininessLocation=matShininessLocation;
}


