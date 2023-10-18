#include "halfedgedisplay.h"
#include "vertex.h"

HalfEdgeDisplay::HalfEdgeDisplay(OpenGLContext* context)
    : Drawable(context), representedHalfEdge(nullptr) {}



void HalfEdgeDisplay::HalfEdgeDisplay::create() {
    std::vector<glm::vec4> pos;
    std::vector<glm::vec4> col;
    std::vector<GLuint> idx;

    if(representedHalfEdge) {
        //start vertex (red)
        pos.push_back(glm::vec4(representedHalfEdge->getVertex()->getPosition(), 1));
        col.push_back(glm::vec4(1, 0, 0, 1));

        //end vertex (yellow)
        pos.push_back(glm::vec4(representedHalfEdge->getNext()->getVertex()->getPosition(), 1));
        col.push_back(glm::vec4(1, 1, 0, 1));

        idx.push_back(0);
        idx.push_back(1);

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
}

void HalfEdgeDisplay::updateHalfEdge(HalfEdge* he) {
    representedHalfEdge = he;
    create();
}

GLenum HalfEdgeDisplay::drawMode(){
    return GL_LINES;
}
