#include "vertex.h"
#include "halfedge.h"

int Vertex::lastID = 0;

Vertex::Vertex(glm::vec3 pos, HalfEdge* he) {
    this->position = pos;
    this->halfEdge = he;
    this->id = lastID++;
}

glm::vec3 Vertex::getPosition() const {
    return position;
}

HalfEdge* Vertex::getHalfEdge() const {
    return halfEdge;
}

int Vertex::getId() const {
    return id;
}

void Vertex::setHalfEdge(HalfEdge* he) {
    halfEdge = he;
    //he->setVertex(this);

}

void Vertex::setPosition(glm::vec3 pos){
    position = pos;
}

