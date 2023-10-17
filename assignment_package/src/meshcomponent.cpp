//#include "MeshComponent.h"

//// Vertex Definitions
//int Vertex::lastID = 0;

//Vertex::Vertex(glm::vec3 pos, HalfEdge* he) {
//    this->position = pos;
//    this->halfEdge = he;
//    this->id = lastID++;
//}

//glm::vec3 Vertex::getPosition() const {
//    return position;
//}

//HalfEdge* Vertex::getHalfEdge() const {
//    return halfEdge;
//}

//int Vertex::getId() const {
//    return id;
//}

//void Vertex::setHalfEdge(HalfEdge* he) {
//    halfEdge = he;
//}

//// Face Definitions
//int Face::lastID = 0;

//Face::Face(const glm::vec3& col) : halfEdge(nullptr), color(col) {
//    id = lastID++;
//}

//glm::vec3 Face::getColor() const {
//    return color;
//}

//HalfEdge* Face::getHalfEdge() const {
//    return halfEdge;
//}

//int Face::getId() const {
//    return id;
//}

//void Face::setHalfEdge(HalfEdge* he) {
//    halfEdge = he;
//}

//// HalfEdge Definitions
//int HalfEdge::lastID = 0;

//HalfEdge::HalfEdge() : next(nullptr), sym(nullptr), face(nullptr), vertex(nullptr) {
//    id = lastID++;
//}

//HalfEdge* HalfEdge::getNext() const {
//    return next;
//}

//HalfEdge* HalfEdge::getSym() const {
//    return sym;
//}

//Face* HalfEdge::getFace() const {
//    return face;
//}

//Vertex* HalfEdge::getVertex() const {
//    return vertex;
//}

//int HalfEdge::getId() const {
//    return id;
//}

//void HalfEdge::setNext(HalfEdge* n) {
//    next = n;
//}

//void HalfEdge::setSym(HalfEdge* s) {
//    sym = s;
//}

//void HalfEdge::setFace(Face* f) {
//    face = f;
//}

//void HalfEdge::setVertex(Vertex* v) {
//    vertex = v;
//}
