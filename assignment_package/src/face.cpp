#include "face.h"

// Face Definitions
int Face::lastID = 0;

Face::Face(const glm::vec3& col) : halfEdge(nullptr), color(col) {
    id = lastID++;
}

glm::vec3 Face::getColor() const {
    return color;
}

HalfEdge* Face::getHalfEdge() const {
    return halfEdge;
}

int Face::getId() const {
    return id;
}

void Face::setHalfEdge(HalfEdge* he) {
    halfEdge = he;
}
