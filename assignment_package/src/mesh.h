#pragma once

#include "drawable.h"
//#include "meshcomponent.h"

#include "face.h"
#include "halfedge.h"
#include "vertex.h"

#include <vector>


class Mesh : public Drawable {

public:
    std::vector<std::unique_ptr<Face>> faces;
    std::vector<std::unique_ptr<HalfEdge>> halfEdges;
    std::vector<std::unique_ptr<Vertex>> vertices;

public:
    Mesh(OpenGLContext* context);

    //Populates the VBOs of the Drawable.
    //Implemented by subclasses of Drawable
    void create() override;


    //Catmull Clark subdivision??
    //step 1: adding new centroids and edge vertices first
    //step 2: moving the original vertices inward
    //step 3?: connecting these vertices with half edges

    //separate function to quadrangulate a face given the new vertices


};

