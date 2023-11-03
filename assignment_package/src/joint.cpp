#include "joint.h"
#include "OpenGL/OpenGL.h"



Joint::Joint(QString& name, glm::vec3 position, glm::quat rotation = glm::quat())
    : name(name), parent(nullptr), position(position), rotation(rotation), hasBindMatrix(false) {


    QTreeWidgetItem::setText(0, name);


}

Joint& Joint::addChild(std::unique_ptr<Joint> child) {
    Joint& childJoint = *child;
    //childJoint.parent = this;
    this->children.push_back(std::move(child));
    QTreeWidgetItem::addChild(children[children.size() - 1].get());
    return childJoint;
}

glm::mat4 Joint::getLocalTransformation() const {
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 orientation = glm::mat4_cast(rotation);
    return translation * orientation;
}

glm::mat4 Joint::getOverallTransformation() const {
    if (parent) {
        return parent->getOverallTransformation() * getLocalTransformation();
    } else {
        return getLocalTransformation();
    }
}


glm::mat4 Joint::setBindMatrix()
{
    //check if joint has a parent
    if (this->parent != nullptr) {
        // bind matrix of the joint is set to the inverse of its local transformation multiplied by the bind matrix of its parent
        this->bindMatrix = glm::inverse(this->getLocalTransformation()) * this->parent->setBindMatrix();
    } else {
        //If the joint doesn't have a parent, the bind matrix is simply set to the inverse of its local transformation.
        this->bindMatrix = glm::inverse(this->getLocalTransformation());
    }
    this->hasBindMatrix = true;
    return this->bindMatrix;
}


void Joint::setJointPosition(int i, double d)
{
    glm::vec4 parentPos;
    if (parent != nullptr) {
        glm::mat4 parentPosMat = glm::mat4(glm::vec4(1, 0, 0, 0),
                                           glm::vec4(0, 1, 0, 0),
                                           glm::vec4(0, 0, 1, 0),
                                           glm::vec4(parent->position, 1));
        parentPosMat = parent->getOverallTransformation();
        parentPos = parentPosMat[3];
    } else {
        parentPos = glm::vec4(0, 0, 0, 1);
    }

    int change = position[i] - (d - parentPos[i]);
    position[i] = d - parentPos[i];

    // need to update the relative positions of the immediate children
    for (auto const & c : children) {
        c->position[i] += change;
    }
}

void Joint::rotateXAxis(double d)
{
    glm::quat q = glm::quat(glm::cos(glm::radians(d) / 2),
                            glm::sin(glm::radians(d) / 2), 0, 0);
    rotation *= q;
}

void Joint::rotateYAxis(double d)
{
    glm::quat q = glm::quat(glm::cos(glm::radians(d) / 2),
                            0, glm::sin(glm::radians(d) / 2), 0);
    rotation *= q;
}

void Joint::rotateZAxis(double d)
{
    glm::quat q = glm::quat(glm::cos(glm::radians(d) / 2),
                            0, 0, glm::sin(glm::radians(d) / 2));
    rotation *= q;
}







