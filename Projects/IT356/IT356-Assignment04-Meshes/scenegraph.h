#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H
#include "group.h"
#include <map>

#include <glm/gtc/type_ptr.hpp>
#include <sstream>

/* Scenegraph class
 *Defines a scenegraph
 *The scenegraph holds a root node and an Animator
 */

class Scenegraph
{

    typedef struct
    {
        GLint positionLocation;
        GLint ambientLocation;
        GLint diffuseLocation;
        GLint specularLocation;
    } LightLocation;

    public:
        //Connstructor when provided object color location, model view location, and a modelview
        //Scenegraph(GLint objectColorLocation, GLint modelviewLocation, glm::mat4& modelview);
        Scenegraph(GLint modelviewLocation, glm::mat4& modelview, GLint normalMatrixLocation, GLint mat_ambientLocation, GLint mat_diffuseLocation, GLint mat_shininessLocation, GLint mat_specularLocation);
        //draw the entire scenegraph
        void draw(stack<glm::mat4> &transformationstack, GLuint program);
        //increase the angle of the animation
        void increaseAngle();
        //DESTRUCTOR
        ~Scenegraph();
        //SETTERS AND GETTERS
        Group* getRoot();
        void setRoot(Group* root);
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
        stack<Group*>& getCurrentGroup();
        void setCurrentGroup(stack<Group*>& currentGroup);
        Animator* getAnimation();
        void setAnimation(Animator* animation);
        vector<Light> getLights(stack<glm::mat4> &transformationstack);
        void enableLights(vector<Light>& lightList, GLuint program);
        vector<LightLocation> lightlocation;
        vector<Light> lightList;
        map<string, GLuint> textureMap;
        GLint textureLocation, textureMatrixLocation;
private:
        //The root of the scenegraph
        Group* root;
        //The location of the object's color
        //GLint objectColorLocation;
        //The location of the modelview
        //GLint modelviewLocation;


        GLint projectionLocation,modelViewLocation;

        GLint numLightsLocation;
        GLint normalMatrixLocation;
        GLint mat_ambientLocation,mat_diffuseLocation,mat_specularLocation,mat_shininessLocation;
        GLint light_ambientLocation,light_diffuseLocation,light_specularLocation,light_positionLocation;

        //The modelview of the scenegraph (viewpoint of the camera)
        glm::mat4 modelview;
        //The stack for the current group
        stack<Group*> currentGroup;
        //The scene's animator
        Animator* animation;

};

#endif // SCENEGRAPH_H
