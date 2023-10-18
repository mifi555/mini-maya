#pragma once

#include <iostream>
#include <memory>
#include <glm/glm.hpp>

#include <QListWidget>
#include <QListWidgetItem>

class HalfEdge;

//**added for GUI, have class inherit fro QListWidgetItem
class Vertex : public QListWidgetItem {
private:
    glm::vec3 position;
    HalfEdge *halfEdge;
    int id;
    static int lastID;

public:
    Vertex(glm::vec3 pos, HalfEdge* he);
    glm::vec3 getPosition() const;
    HalfEdge* getHalfEdge() const;
    int getId() const;
    void setHalfEdge(HalfEdge* he);
};

