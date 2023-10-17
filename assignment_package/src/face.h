#pragma once


#include <iostream>
#include <memory>
#include <glm/glm.hpp>

class HalfEdge;

class Face {
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
};

