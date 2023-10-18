#include "vertexdisplay.h"

VertexDisplay::VertexDisplay(OpenGLContext* context)
    : Drawable(context), representedVertex(nullptr) {}

void VertexDisplay::create() {

    if(!representedVertex) return;

    glm::vec4 pos = glm::vec4(representedVertex->getPosition(), 1);

    //color is white
    glm::vec4 col = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    //drawing one vertex
    count = 1;

    //pos
    generatePos();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    mp_context->glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4), &pos, GL_STATIC_DRAW);

    //color
    generateCol();
    mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    mp_context->glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4), &col, GL_STATIC_DRAW);
}

GLenum VertexDisplay::drawMode(){
    return GL_POINTS;
}


void VertexDisplay::updateVertex(Vertex* v) {
    representedVertex = v;
    create();
}
