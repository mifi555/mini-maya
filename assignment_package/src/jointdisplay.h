#ifndef JOINTDISPLAY_H
#define JOINTDISPLAY_H

#include "joint.h"
#include "drawable.h"

class JointDisplay : public Drawable
{
protected:
    Joint* representedJoint;

public:
    JointDisplay(OpenGLContext *mp_context);

    void create() override;

    void updateJoint(Joint*);

    bool hasAJoint();

    friend class MyGL;
};
#endif // JOINTDISPLAY_H
