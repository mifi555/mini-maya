#ifndef JOINT_H
#define JOINT_H


#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <set>
#include <string>
#include <drawable.h>

class Joint : public Drawable
{

public:
    //Name: The name of this joint which will be displayed in your QTreeWidget of joints.
    std::string name;

    //Parent: The joint that is the parent of this joint.
    Joint* parent;

    //Children: The set of joints that have this joint as their parent.

    std::vector<std::unique_ptr<Joint>> children;

    //Position: The position of this joint relative to its parent joint.
    glm::vec3 position;

    //Rotation: The quaternion that represents this joint's current orientation. You may use glm::quat to represent your quaternion; make sure to include `` in order to access it.
    glm::quat rotation;

    //Bind Matrix: The inverse of the joint's compound transformation matrix at the time a mesh is bound to the joint's skeleton.
    glm::mat4 bindMatrix;

public:
    // Constructor
    Joint(OpenGLContext* context, const std::string& name, glm::vec3 position, glm::quat rotation);

    // Add a child joint
    void addChild(std::unique_ptr<Joint> child);

    // Compute the local transformation matrix
    glm::mat4 getLocalTransformation() const;

    // Compute the overall transformation matrix
    glm::mat4 getOverallTransformation() const;

    void create() override;

    GLenum drawMode() override;

};

#endif // JOINT_H
