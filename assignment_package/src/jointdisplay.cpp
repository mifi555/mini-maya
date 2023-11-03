#include "jointdisplay.h"
#include <iostream>

JointDisplay::JointDisplay(OpenGLContext *mp_context) :
    Drawable(mp_context), representedJoint(nullptr)
{}

void JointDisplay::create()
{
    std::vector<glm::vec4> pos, col;
    std::vector<GLuint> idx;

    int segments = 12; // number of segments
    float radius = 0.5f;

    glm::mat4 currPosMat = representedJoint->getOverallTransformation();
    glm::vec4 currPos = currPosMat[3];
    glm::mat4 rot = glm::toMat4(representedJoint->rotation);

    //white rings
    std::array<glm::vec4, 3> colors = {
        glm::vec4(1, 1, 1, 1),
        glm::vec4(1, 1, 1, 1),
        glm::vec4(1, 1, 1, 1)
    };

    std::array<std::function<glm::vec4(float)>, 3> rotations = {
        [&](float angle) { return glm::vec4(cos(angle) * radius, sin(angle) * radius, 0, 0); },
        [&](float angle) { return glm::vec4(cos(angle) * radius, 0, sin(angle) * radius, 0); },
        [&](float angle) { return glm::vec4(0, sin(angle) * radius, cos(angle) * radius, 0); }
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

void JointDisplay::updateJoint(Joint* j)
{
    representedJoint = j;
    create();
}

bool JointDisplay::hasAJoint()
{
    return representedJoint != nullptr;
}
