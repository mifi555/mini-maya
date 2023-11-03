#ifndef JOINT_H
#define JOINT_H


#include "QtWidgets/qtreewidget.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <set>
#include <string>
#include <drawable.h>

class Joint : public QTreeWidgetItem
{

public:
    //Name: The name of this joint which will be displayed in your QTreeWidget of joints.
    QString name;

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

    bool hasBindMatrix;

public:
    // Constructor
    Joint(QString & name, glm::vec3 position, glm::quat rotation);

    // Add a child joint
    Joint& addChild(std::unique_ptr<Joint> child);

    // Compute the local transformation matrix
    glm::mat4 getLocalTransformation() const;

    // Compute the overall transformation matrix
    glm::mat4 getOverallTransformation() const;

    //calculates and sets the bind matrix for a joint in a skeletal animation system. This matrix represents the joint's transformation relative to its initial (or "bind") position.
    glm::mat4 setBindMatrix();

    //joint transoformations
    void setJointPosition(int, double);
    void rotateXAxis(double);
    void rotateYAxis(double);
    void rotateZAxis(double);

    //friend classes
    friend class Skeleton;
    friend class JointDisplay;
    friend class Mainwindow;

};

#endif // JOINT_H
