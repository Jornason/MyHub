#include "Object.h"

Object::Object(string name, Animator* animation)
    : Node()
{
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    glGenBuffers(NumBuffers,buffers);
    setName(name);
    this->animation = animation;
    texture = glm::mat4(1.0);
}

Object::~Object(void)
{
    //delete the animation
    delete animation;
    if (VAO!=0)
    {
        //clear the buffers
        glDeleteBuffers(NumBuffers,buffers);
        //cleare the VAO
        glDeleteVertexArrays(1,&VAO);
    }
}

void Object::draw(stack<glm::mat4> &transformation, GLuint program)
{
    //Get the object's color
    //glm::vec3 color = this->getColor();

    //tell the shader where the color is
    //glVertexAttrib3fv(scene->getObjectColorLocation(),glm::value_ptr(color));
    //get the transformation matrix
    glm::mat4 currentTransformationMatrix = getTransform();
    //Apply the animations
    currentTransformationMatrix = this->animation->animateThis(getName(), currentTransformationMatrix);
    currentTransformationMatrix = this->animation->yes(getName(), currentTransformationMatrix);
    //Apply the previous transformations
    currentTransformationMatrix =  transformation.top() * currentTransformationMatrix;
    //push the object's transformation to the stack
    transformation.push(currentTransformationMatrix);
    //tell the shader where the transformation is
    //Get the object's material
    glUniformMatrix4fv(scene->textureMatrixLocation,1,GL_FALSE,glm::value_ptr(texture));
    glUniformMatrix4fv(scene->getNormalMatrixLocation(),1,GL_FALSE,glm::value_ptr(glm::transpose(glm::inverse(transformation.top()))));
    glUniform3fv(scene->getmatAmbientLocation(),1,this->getMaterial().getAmbient());
    glUniform3fv(scene->getmatDiffuseLocation(),1,this->getMaterial().getDiffuse());
    glUniform3fv(scene->getmatSpecularLocation(),1,this->getMaterial().getSpecular());
    glUniform1f(scene->getmatShininessLocation(),this->getMaterial().getShininess()[0]);
    glUniformMatrix4fv(scene->getModelviewLocation(),1,GL_FALSE,glm::value_ptr(transformation.top()));


    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textureID);
    glUniform1i(scene->textureLocation,0);
    //bind the VAO
    glBindVertexArray(VAO);
    //draw
    glDrawElements(GL_TRIANGLES,triangleIndices.size(),GL_UNSIGNED_INT,BUFFER_OFFSET(0));
    //pop the transformation
    glDisable(GL_TEXTURE_2D);
    transformation.pop();
}


/*void Object::setColor(float r, float g, float b)
{
    color = glm::vec3(r,g,b);
}

glm::vec3& Object::getColor()
{
    return color;
}*/

void Object::setMaterial(Material material)
{
    this->material = material;
}

Material Object::getMaterial()
{
    return material;
}

Animator* Object::getAnimation()
{
    return animation;
}

void Object::setAnimation(Animator* animation)
{
    this->animation = animation;
}

vector<Light>& Object::getLights(vector<Light>& lightListCopy, stack<glm::mat4>& transformation)
{
    glm::mat4 currentTransformationMatrix = getTransform();
    //Apply the animations
    currentTransformationMatrix = this->animation->animateThis(getName(), currentTransformationMatrix);
    currentTransformationMatrix = this->animation->yes(getName(), currentTransformationMatrix);
    //Apply the previous transformations
    //glm::vec4 currentTransformationVector = transformation.top() * currentTransformationMatrix[3];
    currentTransformationMatrix = transformation.top() * currentTransformationMatrix;
    //push the object's transformation to the stack
    transformation.push(currentTransformationMatrix);
    unsigned int i = 0;


    for(i = 0; i < this->lightList.size(); i++)
    {
        lightListCopy.push_back(lightList[i]);
        if(lightList[i].getPosition()[3] != 0.0f)
            lightListCopy.back().setPosition(glm::vec3(transformation.top() * lightList[i].getPosition()));
        else
            lightListCopy.back().setDirection(glm::vec3(transformation.top() * lightList[i].getPosition()));

    }
    transformation.pop();
    return lightListCopy;
}

