#include "skeleton.h"
#include "joint.h"


#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QQuaternion>

#include <iostream>

Skeleton::Skeleton(OpenGLContext* context) : Drawable(context), joints()
{

}

void Skeleton::create() {
    std::vector<glm::vec4> pos, col;
    std::vector<GLuint> idx;

    createJoints(rootJoint.get(), pos, col, idx);

    count = idx.size();

    generateIdx();
    bindIdx();
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

    generatePos();
    bindPos();
    mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

    generateCol();
    bindCol();
    mp_context->glBufferData(GL_ARRAY_BUFFER, col.size() * sizeof(glm::vec4), col.data(), GL_STATIC_DRAW);

}

//hw07
std::unique_ptr<Joint> Skeleton::readJointsFromJSON(const QJsonObject& jointObject){
    // Extract joint properties from JSON

    QString name = jointObject["name"].toString();

    QJsonArray posArray = jointObject["pos"].toArray();
    QJsonArray rotArray = jointObject["rot"].toArray();

    // Convert rotation array to a quaternion
    float angle = rotArray[0].toDouble();
    glm::vec3 axis(rotArray[1].toDouble(), rotArray[2].toDouble(), rotArray[3].toDouble());
    glm::quat rotation = glm::angleAxis(glm::radians(angle), axis);

    // Create a new joint
    auto joint = std::make_unique<Joint>(name, glm::vec3(posArray[0].toDouble(), posArray[1].toDouble(), posArray[2].toDouble()), rotation);

    //push joint into joint vector

    //joints.push_back(std::unique_ptr<Joint>(joint.get()));

    QJsonArray childrenArray = jointObject["children"].toArray();

    // Process children recursively
    for (int i = 0; i < childrenArray.size(); ++i) {
        auto childJoint = readJointsFromJSON(childrenArray[i].toObject());
        childJoint->parent = joint.get();
        joint->addChild(std::move(childJoint));
    }

    return joint;
}

void Skeleton::createJoints(Joint* joint, std::vector<glm::vec4> &pos,
                           std::vector<glm::vec4> &col, std::vector<GLuint> &idx)
{
    int segments = 12; // number of segments
    float radius = 0.5f;

    glm::mat4 currPosMat = joint->getOverallTransformation();
    glm::vec4 currPos = currPosMat[3];
    glm::mat4 rot = glm::toMat4(joint->rotation);

    std::array<glm::vec4, 3> colors = {
        glm::vec4(1, 0, 0, 1),
        glm::vec4(0, 1, 0, 1),
        glm::vec4(0, 0, 1, 1)
    };

    std::array<std::function<glm::vec4(float)>, 3> rotations = {
        [&](float angle) { return glm::vec4(0, sin(angle) * radius, cos(angle) * radius, 0); },
        [&](float angle) { return glm::vec4(cos(angle) * radius, 0, sin(angle) * radius, 0); },
        [&](float angle) { return glm::vec4(cos(angle) * radius, sin(angle) * radius, 0, 0); }
    };

    int currIdx = pos.size();

    for (int axis = 0; axis < 3; axis++) {
        for (int i = 0; i < segments; i++) {
            float angle = 2.0f * glm::pi<float>() * i / segments;
            glm::vec4 vertex = currPos + (rot * rotations[axis](angle));
            pos.push_back(vertex);
            col.push_back(colors[axis]);
        }

        for (int j = 0; j < segments; j++) {
            idx.push_back(currIdx + j);
            idx.push_back(currIdx + (j + 1) % segments);
        }
        currIdx += segments;
    }

    if (joint->parent != nullptr) {
        glm::mat4 parentPosMat = joint->parent->getOverallTransformation();
        glm::vec4 parentPos = parentPosMat[3];

        pos.push_back(currPos);
        pos.push_back(parentPos);

        col.push_back(glm::vec4(1, 1, 0, 1));
        col.push_back(glm::vec4(1, 0, 1, 1));

        idx.push_back(pos.size() - 2);
        idx.push_back(pos.size() - 1);
    }

    for (auto& child : joint->children) {
        createJoints(child.get(), pos, col, idx);
    }

}

void Skeleton::createJointVector(Joint* j)
{
    if (j == nullptr) {
        return;
    }

    joints.push_back(j);

    for (auto const & c : j->children) {
        createJointVector(c.get());
    }
}

GLenum Skeleton::drawMode(){
    return GL_LINES;
}

void Skeleton::calculateBindMatrices()
{
    for (auto const & joint : joints) {
        if (!joint->hasBindMatrix) {
            joint->setBindMatrix();
        }
    }
}



