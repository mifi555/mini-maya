#include "mygl.h"
#include <la.h>
#include <random>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>

#include <QFileDialog>




MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent),
      m_geomSquare(this),
      m_progLambert(this), m_progFlat(this),
    m_glCamera(), m_mesh(this),
    m_vertDisplay(this),
    m_halfEdgeDisplay(this),
    m_faceDisplay(this)

{
    setFocusPolicy(Qt::StrongFocus);
}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    m_geomSquare.destroy();
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.5, 0.5, 0.5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    //**added**
    m_mesh.create();

    // Create and set up the diffuse shader
    m_progLambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    m_progFlat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");


    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);
}

void MyGL::resizeGL(int w, int h)
{
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    m_glCamera = Camera(w, h);
    glm::mat4 viewproj = m_glCamera.getViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    m_progLambert.setViewProjMatrix(viewproj);
    m_progFlat.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function update() is called, paintGL is called implicitly.
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_progFlat.setViewProjMatrix(m_glCamera.getViewProj());
    m_progLambert.setViewProjMatrix(m_glCamera.getViewProj());
    m_progLambert.setCamPos(m_glCamera.eye);
    m_progFlat.setModelMatrix(glm::mat4(1.f));

    m_progFlat.draw(m_mesh);


    //**added**
    glDisable(GL_DEPTH_TEST);

    //draw components
    if(m_selectVertex){
    m_progFlat.draw(m_vertDisplay);
    }

    if(m_selectHalfEdge){
    m_progFlat.draw(m_halfEdgeDisplay);
    }

    if(m_selectFace){
    m_progFlat.draw(m_faceDisplay);
    }

    glEnable(GL_DEPTH_TEST);


}


void MyGL::keyPressEvent(QKeyEvent *e)
{
    float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
    }
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    // This could all be much more efficient if a switch
    // statement were used
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Right) {
        m_glCamera.RotateAboutUp(-amount);
    } else if (e->key() == Qt::Key_Left) {
        m_glCamera.RotateAboutUp(amount);
    } else if (e->key() == Qt::Key_Up) {
        m_glCamera.RotateAboutRight(-amount);
    } else if (e->key() == Qt::Key_Down) {
        m_glCamera.RotateAboutRight(amount);
    } else if (e->key() == Qt::Key_1) {
        m_glCamera.fovy += amount;
    } else if (e->key() == Qt::Key_2) {
        m_glCamera.fovy -= amount;
    } else if (e->key() == Qt::Key_W) {
        m_glCamera.TranslateAlongLook(amount);
    } else if (e->key() == Qt::Key_S) {
        m_glCamera.TranslateAlongLook(-amount);
    } else if (e->key() == Qt::Key_D) {
        m_glCamera.TranslateAlongRight(amount);
    } else if (e->key() == Qt::Key_A) {
        m_glCamera.TranslateAlongRight(-amount);
    } else if (e->key() == Qt::Key_Q) {
        m_glCamera.TranslateAlongUp(-amount);
    } else if (e->key() == Qt::Key_E) {
        m_glCamera.TranslateAlongUp(amount);
    } else if (e->key() == Qt::Key_R) {
        m_glCamera = Camera(this->width(), this->height());
    }

    //**Visual Debugging Tools: added keys

    //N: NEXT half-edge of the currently selected half-edge
    else if(e->key() == Qt::Key_N) {
        if(m_selectedHalfEdgePtr) {
            m_selectedHalfEdgePtr = m_selectedHalfEdgePtr->getNext();

            m_selectHalfEdge = true;
            m_selectVertex = false;
            m_selectFace = false;

            m_halfEdgeDisplay.updateHalfEdge(m_selectedHalfEdgePtr);
        }
        //M: SYM half-edge of the currently selected half-edge
    } else if(e->key() == Qt::Key_M) {
        if(m_selectedHalfEdgePtr) {
            m_selectedHalfEdgePtr = m_selectedHalfEdgePtr->getSym();

            m_selectHalfEdge = true;
            m_selectVertex = false;
            m_selectFace = false;

            m_halfEdgeDisplay.updateHalfEdge(m_selectedHalfEdgePtr);
        }
        //F: FACE of the currently selected half-edge
    } else if(e->key() == Qt::Key_F) {
        if(m_selectedHalfEdgePtr) {
            m_selectedFacePtr = m_selectedHalfEdgePtr->getFace();

            m_selectFace = true;
            m_selectHalfEdge = false;
            m_selectVertex = false;
            m_faceDisplay.updateFace(m_selectedFacePtr);
        }
        //V: VERTEX of the currently selected half-edge
    } else if(e->key() == Qt::Key_V) {
        if(m_selectedHalfEdgePtr) {
            m_selectedVertexPtr = m_selectedHalfEdgePtr->getVertex();

            m_selectVertex = true;
            m_selectHalfEdge = false;
            m_selectFace = false;

            m_vertDisplay.updateVertex(m_selectedVertexPtr);
        }
    } else if(e->key() == Qt::Key_H) {
        //Shift + H: HALF-EDGE of the currently selected face
        if(e->modifiers() & Qt::ShiftModifier) {
            if(m_selectedFacePtr) {
                m_selectedHalfEdgePtr = m_selectedFacePtr->getHalfEdge();

                m_selectHalfEdge = true;
                m_selectVertex = false;
                m_selectFace = false;
                m_halfEdgeDisplay.updateHalfEdge(m_selectedHalfEdgePtr);
            }
            //H: HALF-EDGE of the currently selected vertex
        } else {
            if(m_selectedVertexPtr) {
                m_selectedHalfEdgePtr = m_selectedVertexPtr->getHalfEdge();
                m_selectHalfEdge = true;
                m_selectVertex = false;
                m_selectFace = false;
                m_halfEdgeDisplay.updateHalfEdge(m_selectedHalfEdgePtr);
            }
        }
    }
    m_glCamera.RecomputeAttributes();

    update();  // Calls paintGL, among other things
}


//**added**

    void MyGL::slot_loadOBJFile(const QString &fileName) {

        //vectors to store read vertices and faces
        std::vector<glm::vec3> vertexPositions;
        std::vector<std::vector<int>> faces;

        //read obj file
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Failed to open" << fileName;
            return;
        }

        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(" ");
            if (parts[0] == "v") {
                vertexPositions.push_back(glm::vec3(parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat()));
            } else if (parts[0] == "f") {
                std::vector<int> face;
                for (int i = 1; i < parts.size(); i++) {
                    QStringList subparts = parts[i].split("/");
                    face.push_back(subparts[0].toInt() - 1);
                }
                faces.push_back(face);
            }
        }

        file.close();
        //create half edge data structure

        //map of vertex pair-half edge : used for finding symmetric half edges
        std::map<std::pair<int, int>, HalfEdge*> halfEdgeMap;

        //* vertex map
        std::unordered_map<int, Vertex*> vertexMap;


        //faceIndices is a vertex of indices that define a single face
        for (auto& faceIndices : faces){
            //create face with random color
            auto face = std::make_unique<Face>(generateRandomColor());

            //firstHalfEdge gives us a reference to the beginning of the list of half-edges for a face
            //prevHalfEdge helps us connect each half-edge to the next one as we loop through the vertices of the face
            HalfEdge *firstHalfEdge = nullptr;
            //prevhalfedge points to the last created half edge
            HalfEdge *prevHalfEdge = nullptr;

            //faceIndices.size(): number of vertices that constitute that face.
            for (size_t i = 0; i < faceIndices.size(); i++){

                auto halfEdge = std::make_unique<HalfEdge>();

                int idx1 = faceIndices[i];
                int idx2 = faceIndices[(i + 1) % faceIndices.size()];


                //**
//                //make vertex unique pointer
//                auto vertex = std::make_unique<Vertex>(vertexPositions[idx2], nullptr);
//                //push vertex into m_mesh's vector of vertices
//                m_mesh.vertices.push_back(std::move(vertex));
//                //set face and vertex for half edge
//                halfEdge->setFace(face.get());
//                halfEdge->setVertex(m_mesh.vertices.back().get());
               //**

                //prevent from creating duplicate vertices
                Vertex* vertexPtr;
                //if we haven't created a vertex for a particular index, you create it, store it in your mesh's vertices list, and update the map.
                if(vertexMap.find(idx2) == vertexMap.end()) {
                    auto vertex = std::make_unique<Vertex>(vertexPositions[idx2], nullptr);
                    m_mesh.vertices.push_back(std::move(vertex));
                    vertexPtr = m_mesh.vertices.back().get();
                    vertexMap[idx2] = vertexPtr;

                //if we've already created a vertex for a particular index, you simply retrieve it from the map and use it.
                } else {
                    vertexPtr = vertexMap[idx2];
                }
                halfEdge->setFace(face.get());
                halfEdge->setVertex(vertexPtr);

                //set up next relationships between halfedges

                //on the first iteration, we assign firstHalfEdge to our newly created halfEdge
                if(firstHalfEdge == nullptr){
                    firstHalfEdge = halfEdge.get();
                }
                if(prevHalfEdge != nullptr){
                    //on later iterations, set next pointer of the prevhalfedge to the current halfedge
                    prevHalfEdge->setNext(halfEdge.get());
                }

                //close the loop for half-edges of this face
                prevHalfEdge = halfEdge.get();

                //Setting up SYM pointers
                auto it = halfEdgeMap.find({idx2, idx1});


                //std::cout << "Key for HE " << halfEdge->getId() << " is {" << smallID << ", " << largeID << "}." << std::endl;

                if(it != halfEdgeMap.end()){
                    //std::cout << "Found KVP for pair {" << smallID << ", " << largeID << "}. Value was HE " << it->second->getId() << std::endl;
                    //.second retrieves value aka the half edge
                    halfEdge->setSym(it->second);
                    it->second->setSym(halfEdge.get());
                    //it->second->setVertex(vertexMap[idx1]);
                }
                //push half edge into mesh's vector
                m_mesh.halfEdges.push_back(std::move(halfEdge));

                //if we reached the end
                if (it == halfEdgeMap.end()){
                    halfEdgeMap[{idx1, idx2}] = m_mesh.halfEdges.back().get();
                }
            }
            //close the loop by having the prevHalfEdge point to the first half edge
            prevHalfEdge->setNext(firstHalfEdge);

            //set face of half edge
            face->setHalfEdge(firstHalfEdge);
            //push face into mesh's face vector
            m_mesh.faces.push_back(std::move(face));
        }

        //create the mesh
        m_mesh.create();

    }

glm::vec3 MyGL::generateRandomColor() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    float red = dist(gen);
    float green = dist(gen);
    float blue = dist(gen);

    return glm::vec3(red, green, blue);
}
