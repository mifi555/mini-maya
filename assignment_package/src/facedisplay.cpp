#include "facedisplay.h"

#include "halfedge.h"
#include "vertex.h"

FaceDisplay::FaceDisplay(OpenGLContext* context)
    : Drawable(context), representedFace(nullptr) {}

void FaceDisplay::create() {
    if(!representedFace) return;

    std::vector<glm::vec4> pos;
    std::vector<glm::vec4> col;
    std::vector<GLuint> idx;


    HalfEdge* start_edge = representedFace->getHalfEdge();
    HalfEdge* current_edge = start_edge;
    glm::vec4 faceColor = glm::vec4(representedFace->getColor(), 1);
    glm::vec4 oppositeColor = glm::vec4(1.0f - faceColor.x, 1.0f - faceColor.y, 1.0f - faceColor.z, 1.0f); // Opposite color


    int i = 0;
    do {
        //highlight all of the edges around the faces

        //start pos of edge
        pos.push_back(glm::vec4(current_edge->getVertex()->getPosition(), 1));
        //end pos of edge
        pos.push_back(glm::vec4(current_edge->getNext()->getVertex()->getPosition(), 1));

        //color for start pos
        col.push_back(oppositeColor);
        //col for end pos
        col.push_back(oppositeColor);

        //index for start pos
        idx.push_back(2 * i);
        //index for end position
        idx.push_back(2 * i + 1);

        current_edge = current_edge->getNext();
        i++;
    } while (current_edge != start_edge);

    count = idx.size();

    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, col.size() * sizeof(glm::vec4), col.data(), GL_STATIC_DRAW);

    generateIdx();
    mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

}

void FaceDisplay::updateFace(Face* f) {
    representedFace = f;
    create();
}

GLenum FaceDisplay::drawMode(){
    return GL_LINES;
}
