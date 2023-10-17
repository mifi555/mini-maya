#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>

#include <QFileDialog>




MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent),
      m_geomSquare(this),
      m_progLambert(this), m_progFlat(this),
    m_glCamera(), m_mesh(this)

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

    //Create the instances of Cylinder and Sphere.
//    m_geomSquare.create();

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

    //Create a model matrix. This one rotates the square by PI/4 radians then translates it by <-2,0,0>.
    //Note that we have to transpose the model matrix before passing it to the shader
    //This is because OpenGL expects column-major matrices, but you've
    //implemented row-major matrices.
//    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-2,0,0)) * glm::rotate(glm::mat4(), 0.25f * 3.14159f, glm::vec3(0,1,0));
//    //Send the geometry's transformation matrix to the shader
//    m_progLambert.setModelMatrix(model);
//    //Draw the example sphere using our lambert shader
//    m_progLambert.draw(m_geomSquare);

//    //Now do the same to render the cylinder
//    //We've rotated it -45 degrees on the Z axis, then translated it to the point <2,2,0>
//    model = glm::translate(glm::mat4(1.0f), glm::vec3(2,2,0)) * glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0,0,1));
//    m_progLambert.setModelMatrix(model);
//    m_progLambert.draw(m_geomSquare);

    m_progFlat.draw(m_mesh);
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
    m_glCamera.RecomputeAttributes();
    update();  // Calls paintGL, among other things
}


//**added**

void MyGL::slot_loadOBJFile(const QString &fileName) {
    //vectors to store read vertices and faces
    std::vector<glm::vec3> vertices;
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
            vertices.push_back(glm::vec3(parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat()));
        } else if (parts[0] == "f") {
            std::vector<int> face;
            for (int i = 1; i < parts.size(); i++) {
                QStringList subparts = parts[i].split("/");
                face.push_back(subparts[0].toInt() - 1);  // OBJ indices start at 1
            }
            faces.push_back(face);
        }
    }

    file.close();

//    for (int i = 0; i < faces.size(); i++){
//        for (int j = 0; j < faces[i].size(); j++){
//            std::cout << faces[i][j] <<  std::endl;
//        }
//    }

    //create half edge data structure

    //map of vertex pair : half edge
    //used for finding symmetric half edges
    std::map<std::pair<int, int>, HalfEdge*> halfEdgeMap;

    //faceIndices is a vertex of indices that define a single face.
    for (auto& faceIndices : faces){
        //create face with random color
        auto face = std::make_unique<Face>(glm::vec3(0.5, 0.5, 0.5));

        //process pair of vertices
        //for each pair of vertices a half edge is made

        //firstHalfEdge gives us a reference to the beginning of the list of half-edges for a face.
        //prevHalfEdge helps us connect each half-edge to the next one as we loop through the vertices of the face.
        HalfEdge *firstHalfEdge = nullptr;
        //prevhalfedge points to the last created half edge
        HalfEdge *prevHalfEdge = nullptr;

        //faceIndices.size(): number of vertices that constitute that face.
        for (size_t i = 0; i < faceIndices.size(); i++){

            //A new half-edge is instantiated
            //two consecutive indices

            auto halfEdge = std::make_unique<HalfEdge>();

            int idx1 = faceIndices[i];
            int idx2 = faceIndices[(i + 1)] % faceIndices.size();

            //set face and vertex for half edge
            halfEdge->setFace(face.get());
            halfEdge->setVertex(new Vertex(vertices[idx2], halfEdge.get()));

            //set up next relationships between halfedges
            //half-edge (newHE) currently being created is the first half-edge of the face

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
            //finds position where the key is present in the map
            auto it = halfEdgeMap.find({idx2, idx1});

            if(it != halfEdgeMap.end()){
                //.second retrieves value aka the half edge
                halfEdge->setSym(it->second);
                it->second->setSym(halfEdge.get());
            }

            //push half edge into mesh's vector
            m_mesh.halfEdges.push_back(std::move(halfEdge));


            //if we reached the end
            if (it == halfEdgeMap.end()){
                halfEdgeMap[{idx1, idx2}] = m_mesh.halfEdges.back().get();
            }

            //std::cout << m_mesh.halfEdges.back().get()->getVertex()->getId() << std::endl;

            //push half edge into mesh's vector
            //m_mesh.halfEdges.push_back(std::move(halfEdge));
        }
        //close the loop by having the prevHalfEdge point to the first half edge
        prevHalfEdge->setNext(firstHalfEdge);

        //set face of half edge
        face->setHalfEdge(firstHalfEdge);
        //push face into mesh's face vector
        m_mesh.faces.push_back(std::move(face));
    }
        //iterate over vertices and push them into the mesh's vertex vector
        for (const auto& v : vertices) {
            auto vertex = std::make_unique<Vertex>(v, nullptr);
            m_mesh.vertices.push_back(std::move(vertex));
        }

//        //set the half-edge pointer for each vertex.
//        for (const auto& he : m_mesh.halfEdges) {
//            Vertex* vertex = he->getVertex();
//            if (vertex->getHalfEdge() == nullptr) {
//                vertex->setHalfEdge(he.get());
//            }
//        }
        std::cout << "number of vertices: " << m_mesh.vertices.size()<< std::endl;

    //std::cout << m_mesh.faces.back().get()->getHalfEdge()->getId()<< std::endl;

    //create the mesh
    m_mesh.create();

}
