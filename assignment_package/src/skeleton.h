#ifndef SKELETON_H
#define SKELETON_H

#include "drawable.h"
#include "joint.h"



class Skeleton : public Drawable
{

private:
    std::unique_ptr<Joint> rootJoint;
    std::vector<Joint*> joints;


public:

    Skeleton(OpenGLContext* context);
    void create() override;
    GLenum drawMode() override;

    std::unique_ptr<Joint> readJointsFromJSON(const QJsonObject& jointObject);
    void createJoints(Joint *joint, std::vector<glm::vec4> &pos, std::vector<glm::vec4> &col, std::vector<GLuint> &idx);
    void createJointVector(Joint *joint);

    //skinning
    void calculateBindMatrices();

    friend class MyGL;
    friend class MainWindow;
};

#endif // SKELETON_H
