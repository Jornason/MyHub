#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H
#include "group.h"
#include <glm/gtc/type_ptr.hpp>

/* Scenegraph class
 *Defines a scenegraph
 *The scenegraph holds a root node and an Animator
 */

class Scenegraph
{
    public:
        //Connstructor when provided object color location, model view location, and a modelview
        Scenegraph(GLint objectColorLocation, GLint modelviewLocation, glm::mat4& modelview);
        //draw the entire scenegraph
        void draw(stack<glm::mat4> &transformationstack);
        //increase the angle of the animation
        void increaseAngle();
        //DESTRUCTOR
        ~Scenegraph();
        //SETTERS AND GETTERS
        Group* getRoot();
        void setRoot(Group* root);
        GLint getObjectColorLocation();
        void setObjectColorLocation(GLint objectColorLocation);
        GLint getModelviewLocation();
        void setModelviewLocation(GLint modelviewLocation);
        glm::mat4& getModelview();
        void setModelview(glm::mat4& modelview);
        stack<Group*>& getCurrentGroup();
        void setCurrentGroup(stack<Group*>& currentGroup);
        Animator* getAnimation();
        void setAnimation(Animator* animation);
private:
        //The root of the scenegraph
        Group* root;
        //The location of the object's color
        GLint objectColorLocation;
        //The location of the modelview
        GLint modelviewLocation;
        //The modelview of the scenegraph (viewpoint of the camera)
        glm::mat4 modelview;
        //The stack for the current group
        stack<Group*> currentGroup;
        //The scene's animator
        Animator* animation;
};

#endif // SCENEGRAPH_H
