#pragma once
#include <iostream>
#include <memory>
#include <glm/glm.hpp>

#include <QListWidget>
#include <QListWidgetItem>

class Face;
class Vertex;

//**added for GUI, have class inherit fro QListWidgetItem
class HalfEdge : public QListWidgetItem{
private:
    HalfEdge *next;
    HalfEdge *sym;
    Face *face;
    Vertex *vertex;
    int id;
    static int lastID;

public:
    HalfEdge();
    HalfEdge* getNext() const;
    HalfEdge* getSym() const;
    Face* getFace() const;
    Vertex* getVertex() const;
    int getId() const;
    void setNext(HalfEdge* n);
    void setSym(HalfEdge* s);
    void setFace(Face* f);
    void setVertex(Vertex* v);
};


