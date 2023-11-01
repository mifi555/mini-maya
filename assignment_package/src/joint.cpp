#include "joint.h"
#include "OpenGL/OpenGL.h"



Joint::Joint(OpenGLContext* context, const std::string& name, glm::vec3 position, glm::quat rotation = glm::quat())
    : Drawable(context), name(name), parent(nullptr), position(position), rotation(rotation) {}

void Joint::addChild(std::unique_ptr<Joint> child) {
//    children.push_back(child);
//    child->parent = this;
    Joint& childJoint = *child;
    childJoint.parent = this;

    this->children.push_back(std::move(child));
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

void Joint::create(){
    std::vector<glm::vec4> pos;
    std::vector<glm::vec4> col;
    std::vector<GLuint> idx;

    int angle = 0;
    int id = 0;

    int segments = 12; //number of segments
    float radius = 0.5f;

    //12 line segments, for loop depends on angle size
//    for(int i = 0; i < segments; i++){
//        angle += 45;
//        glm::vec4 position = glm::vec4(0, sin(i * angle) * radius, cos(i * angle) * radius, 1.0);
//        //red color
//        glm::vec4 color = glm::vec4(1, 0, 0, 1);

//        idx.push_back(id);
//        col.push_back(color);
//        idx.push_back(id);
//    }

    // Rotation around X-axis (Red circle)
    for (int i = 0; i < segments; i++) {
        //2.0f * glm::pi<float>: full circle
        //calculates the angle in radians for the current segment, based on its position in the loop
        float angle = 2.0f * glm::pi<float>() * i / segments;
        glm::vec4 vertex = glm::vec4(0, sin(angle) * radius, cos(angle) * radius, 1.0);
        pos.push_back(vertex);
        col.push_back(glm::vec4(1, 0, 0, 1));
    }

    // Rotation around Y-axis (Green circle)
    for (int i = 0; i < segments; i++) {
        float angle = 2.0f * glm::pi<float>() * i / segments;
        glm::vec4 vertex = glm::vec4(cos(angle) * radius, 0, sin(angle) * radius, 1.0);
        pos.push_back(vertex);
        col.push_back(glm::vec4(0, 1, 0, 1));
    }

    // Rotation around Z-axis (Blue circle)
    for (int i = 0; i < segments; i++) {
        float angle = 2.0f * glm::pi<float>() * i / segments;
        glm::vec4 vertex = glm::vec4(cos(angle) * radius, sin(angle) * radius, 0, 1.0);
        pos.push_back(vertex);
        col.push_back(glm::vec4(0, 0, 1, 1));
    }

    // Creating indices for the wireframe

    //we create pairs of indices to represent lines between the vertices. This loop ensures each vertex is connected to the next, creating the wireframe appearance
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < segments; j++) {
            idx.push_back(i * segments + j);
            idx.push_back(i * segments + (j + 1) % segments);
        }
    }

    //if joint has a parent
    if(parent) {
        // Compute the global transformations for this joint and its parent
        glm::mat4 thisGlobalTransform = getOverallTransformation();
        glm::mat4 parentGlobalTransform = parent->getOverallTransformation();

        // Get the positions for this joint and its parent in world space
        glm::vec4 thisPosWorld = thisGlobalTransform * glm::vec4(0,0,0,1);
        glm::vec4 parentPosWorld = parentGlobalTransform * glm::vec4(0,0,0,1);

        // Push these positions to the pos vector
        pos.push_back(thisPosWorld);
        pos.push_back(parentPosWorld);

        // Push the respective colors to the col vector
        col.push_back(glm::vec4(1,1,0,1));  // Yellow for this joint (child)
        col.push_back(glm::vec4(1,0,1,1));  // Magenta for parent joint

        // Push indices to draw a line between the added vertices
        idx.push_back(pos.size() - 2);  // Index for this joint
        idx.push_back(pos.size() - 1);  // Index for parent
    }

    //create spheres in draw:

    //current position on circle * getoverall transformation of joints

    //draw line from child to parent
    count = idx.size();

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, col.size() * sizeof(glm::vec4), col.data(), GL_STATIC_DRAW);


}

GLenum Joint::drawMode(){
    return GL_LINES;
}






