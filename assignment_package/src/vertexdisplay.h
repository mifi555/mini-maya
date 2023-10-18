#ifndef VERTEXDISPLAY_H
#define VERTEXDISPLAY_H

#include "drawable.h"
#include "vertex.h"

class VertexDisplay : public Drawable {

protected:
    Vertex *representedVertex;

public:

    VertexDisplay(OpenGLContext* context);
    // Creates VBO data to make a visual
    // representation of the currently selected Vertex
    void create() override;

    GLenum drawMode() override;

    // Change which Vertex representedVertex points to
    void updateVertex(Vertex*);
};

#endif // VERTEXDISPLAY_H
