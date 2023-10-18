#include "halfedge.h"
#include "vertex.h"
// HalfEdge Definitions
int HalfEdge::lastID = 0;

HalfEdge::HalfEdge() : next(nullptr), sym(nullptr), face(nullptr), vertex(nullptr) {
    id = lastID++;
}

HalfEdge* HalfEdge::getNext() const {
    return next;
}

HalfEdge* HalfEdge::getSym() const {
    return sym;
}

Face* HalfEdge::getFace() const {
    return face;
}

Vertex* HalfEdge::getVertex() const {
    return vertex;
}

int HalfEdge::getId() const {
    return id;
}

void HalfEdge::setNext(HalfEdge* n) {
    next = n;
}

void HalfEdge::setSym(HalfEdge* s) {
    sym = s;
}

void HalfEdge::setFace(Face* f) {
    face = f;
}

void HalfEdge::setVertex(Vertex* v) {
    vertex = v;
    v->setHalfEdge(this);
}
