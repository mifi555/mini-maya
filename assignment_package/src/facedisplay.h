#ifndef FACEDISPLAY_H
#define FACEDISPLAY_H

#include "face.h"
#include "drawable.h"

class FaceDisplay : public Drawable {

protected:
    Face *representedFace;

public:
    FaceDisplay(OpenGLContext* context);

    void create() override;
    void updateFace(Face*);

    GLenum drawMode() override;
};

#endif // FACEDISPLAY_H
