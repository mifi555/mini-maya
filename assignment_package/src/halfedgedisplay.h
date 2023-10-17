#ifndef HALFEDGEDISPLAY_H
#define HALFEDGEDISPLAY_H

#include "halfedge.h"
#include "drawable.h"

class HalfEdgeDisplay : public Drawable {
protected:
    HalfEdge *representedHalfEdge;

public:
    void create() override;
    void updateHalfEdge(HalfEdge*);
};

#endif // HALFEDGEDISPLAY_H
