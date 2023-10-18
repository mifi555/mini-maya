#ifndef HALFEDGEDISPLAY_H
#define HALFEDGEDISPLAY_H

#include "halfedge.h"
#include "drawable.h"

class HalfEdgeDisplay : public Drawable {
protected:
    HalfEdge *representedHalfEdge;

public:
    HalfEdgeDisplay(OpenGLContext* context);

    void create() override;
    void updateHalfEdge(HalfEdge*);

    GLenum drawMode() override;
};

#endif // HALFEDGEDISPLAY_H
