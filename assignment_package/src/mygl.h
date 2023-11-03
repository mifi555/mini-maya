#ifndef MYGL_H
#define MYGL_H

#include <openglcontext.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/squareplane.h>
#include "camera.h"
#include "mesh.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

//#include <glm/glm.hpp>

#include "vertexdisplay.h"
#include "halfedgedisplay.h"
#include "facedisplay.h"

#include "skeleton.h"

#include "joint.h"
#include "jointdisplay.h"

class MyGL
    : public OpenGLContext
{
    Q_OBJECT
private:
    SquarePlane m_geomSquare;// The instance of a unit cylinder we can use to render any cylinder
    ShaderProgram m_progLambert;// A shader program that uses lambertian reflection
    ShaderProgram m_progFlat;// A shader program that uses "flat" reflection (no shadowing at all)
    ShaderProgram m_progSkeleton; //shader program that deforms mesh based on skeleton deformation

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.

    Camera m_glCamera;

    //**added**
    Mesh m_mesh;

    //**Graphical User Interface displays
    VertexDisplay m_vertDisplay;
    HalfEdgeDisplay m_halfEdgeDisplay;
    FaceDisplay m_faceDisplay;
    JointDisplay m_jointDisplay;

    //**Joints
    Skeleton m_skeleton;
    Joint* m_rootJoint;
    std::vector<std::unique_ptr<Joint>> m_joints;

    bool loadedObj;
    bool loadedJoints;

public:
    explicit MyGL(QWidget *parent = nullptr);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    //**added**
    glm::vec3 generateRandomColor();

    //**Vertex, HalfEdge, Face selection flags for paintGL
    bool m_selectVertex = false;
    bool m_selectHalfEdge = false;
    bool m_selectFace = false;
    bool m_selectJoint = false;


    //**Visual Debugging Tools
    HalfEdge* m_selectedHalfEdgePtr = nullptr;
    Vertex* m_selectedVertexPtr = nullptr;
    Face* m_selectedFacePtr = nullptr;
    Joint* m_selectedJointPtr = nullptr;

    //Joints
    void loadJSONFile(const QString& filePath);
    void giveJointWeights();

    friend class MainWindow;


public slots:
    void slot_loadOBJFile(const QString &fileName);

    //hw07
    void slot_bindSkeleton();

    //slots for Interactive Skeleton
    void slot_modifyJointPosX(double);
    void slot_modifyJointPosY(double);
    void slot_modifyJointPosZ(double);

    void slot_rotatePositiveX();
    void slot_rotatePositiveY();
    void slot_rotatePositiveZ();
    void slot_rotateNegativeX();
    void slot_rotateNegativeY();
    void slot_rotateNegativeZ();


signals:
    void sig_sendRootNode(QTreeWidgetItem*);

protected:
    void keyPressEvent(QKeyEvent *e);
};


#endif // MYGL_H
