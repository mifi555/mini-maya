#pragma once


#include <QTreeWidgetItem>
#include <iostream>
#include <memory>
#include <glm/glm.hpp>

#include <QListWidget>
#include <QListWidgetItem>

class HalfEdge;

//**added for GUI, have class inherit fro QListWidgetItem
class Face : public QListWidgetItem {
private:
    HalfEdge *halfEdge;
    glm::vec3 color;
    int id;
    static int lastID;

public:
    Face(const glm::vec3& col);
    glm::vec3 getColor() const;
    HalfEdge* getHalfEdge() const;
    int getId() const;
    void setHalfEdge(HalfEdge* he);
    void setColor(glm::vec3 newColor);
};

