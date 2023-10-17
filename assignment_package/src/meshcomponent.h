//#pragma once

//#include <iostream>
//#include <memory>
//#include <glm/glm.hpp>

//// Forward declarations
//class Vertex;
//class Face;
//class HalfEdge;

//class Vertex {
//private:
//    glm::vec3 position;
//    HalfEdge *halfEdge;
//    int id;
//    static int lastID;

//public:
//    Vertex(glm::vec3 pos, HalfEdge* he);
//    glm::vec3 getPosition() const;
//    HalfEdge* getHalfEdge() const;
//    int getId() const;
//    void setHalfEdge(HalfEdge* he);
//};

//class Face {
//private:
//    HalfEdge *halfEdge;
//    glm::vec3 color;
//    int id;
//    static int lastID;

//public:
//    Face(const glm::vec3& col);
//    glm::vec3 getColor() const;
//    HalfEdge* getHalfEdge() const;
//    int getId() const;
//    void setHalfEdge(HalfEdge* he);
//};

//class HalfEdge {
//private:
//    HalfEdge *next;
//    HalfEdge *sym;
//    Face *face;
//    Vertex *vertex;
//    int id;
//    static int lastID;

//public:
//    HalfEdge();
//    HalfEdge* getNext() const;
//    HalfEdge* getSym() const;
//    Face* getFace() const;
//    Vertex* getVertex() const;
//    int getId() const;
//    void setNext(HalfEdge* n);
//    void setSym(HalfEdge* s);
//    void setFace(Face* f);
//    void setVertex(Vertex* v);
//};

