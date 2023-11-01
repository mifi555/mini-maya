#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "cameracontrolshelp.h"

#include <QFileDialog>

#include <set>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocus();

    //**added**
    connect(ui->importObj,
            //signal name
            SIGNAL(clicked()),
            //signal receiver
            this,
            //slot name
            SLOT(slot_openFile()));


    //clicking on vertex
    connect(ui->vertsListWidget,
            SIGNAL(itemClicked(QListWidgetItem*)),
            this,
            SLOT(slot_vertexSelected(QListWidgetItem*)));

    connect(ui->halfEdgesListWidget,
            SIGNAL(itemClicked(QListWidgetItem*)),
            this,
            SLOT(slot_halfEdgeSelected(QListWidgetItem*)));

    connect(ui->facesListWidget,
            SIGNAL(itemClicked(QListWidgetItem*)),
            this,
            SLOT(slot_faceSelected(QListWidgetItem*)));

    //add vertex
    connect(ui->addVertex,
            SIGNAL(clicked()),
            this,
            SLOT(slot_addVertexToSelectedHalfEdge()));

    connect(ui->triangulateFace, SIGNAL(clicked()), this, SLOT(slot_triangulateSelectedFace()));


    //spinboxes

    //faceRedSpinBox
    connect(ui->faceRedSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_modifyFaceColorRed(double)));
    //faceGreenSpinBox
    connect(ui->faceGreenSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_modifyFaceColorGreen(double)));
    //faceBlueSpinBox
    connect(ui->faceBlueSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_modifyFaceColorBlue(double)));


    //modify vertex position
    connect(ui->vertPosXSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_modifyVertexPositionX(double)));
    connect(ui->vertPosYSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_modifyVertexPositionY(double)));
    connect(ui->vertPosZSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_modifyVertexPositionZ(double)));

    //Catmull-Clark subdivision
    connect(ui->subdivide, SIGNAL(clicked()), this, SLOT(slot_ccSubdivide()));

    //hw07: load skeleton
    connect(ui->skeletonButton, SIGNAL(clicked()), this, SLOT(slot_loadSkeleton()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionCamera_Controls_triggered()
{
    CameraControlsHelp* c = new CameraControlsHelp();
    c->show();
}

//*added*

void MainWindow::slot_openFile(){
     QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString("../../../../obj_files"), tr("OBJ Files (*.obj)"));
        if(!fileName.isEmpty()) {
            ui->mygl->slot_loadOBJFile(fileName);

            //add mesh components to lists of widgets
            for (auto &vertex : ui->mygl->m_mesh.vertices){
                ui->vertsListWidget->addItem(QString::number(vertex->getId()));
            }
            for (auto &face : ui->mygl->m_mesh.faces){
                ui->facesListWidget->addItem(QString::number(face->getId()));
            }
            for (auto &halfEdge : ui->mygl->m_mesh.halfEdges){
                ui->halfEdgesListWidget->addItem(QString::number(halfEdge->getId()));
            }
        }
}

void MainWindow::slot_vertexSelected(QListWidgetItem *item){
        Vertex *vertex = nullptr;
        for (auto &v : ui->mygl->m_mesh.vertices) {
            if (v.get()->getId() == item->text().toInt()){
                vertex = v.get();
                break;
            }
        }
        if(vertex) {
            ui->mygl->m_selectedVertexPtr = vertex;
            ui->mygl->m_selectedHalfEdgePtr = nullptr;
            ui->mygl->m_selectedFacePtr = nullptr;

            ui->mygl->m_selectVertex = true;
            ui->mygl->m_selectHalfEdge = false;
            ui->mygl->m_selectFace = false;

            ui->mygl->m_vertDisplay.updateVertex(vertex);
            ui->mygl->update();
        }
}

void MainWindow::slot_halfEdgeSelected(QListWidgetItem *item){
        HalfEdge *halfEdge = nullptr;
        for (auto &h : ui->mygl->m_mesh.halfEdges) {
            if (h.get()->getId() == item->text().toInt()){
                halfEdge = h.get();
                break;
            }
        }
        if(halfEdge) {
            ui->mygl->m_selectedVertexPtr = nullptr;
            ui->mygl->m_selectedHalfEdgePtr = halfEdge;
            ui->mygl->m_selectedFacePtr = nullptr;

            ui->mygl->m_selectHalfEdge = true;
            ui->mygl->m_selectVertex = false;
            ui->mygl->m_selectFace = false;
            ui->mygl->m_halfEdgeDisplay.updateHalfEdge(halfEdge);
            ui->mygl->update();
        }
}

void MainWindow::slot_faceSelected(QListWidgetItem *item){
        Face *face = nullptr;
        for (auto &f : ui->mygl->m_mesh.faces) {
            if (f.get()->getId() == item->text().toInt()){
                face = f.get();
                break;
            }
        }
        if(face) {
            ui->mygl->m_selectedVertexPtr = nullptr;
            ui->mygl->m_selectedHalfEdgePtr = nullptr;
            ui->mygl->m_selectedFacePtr = face;


            ui->mygl->m_selectFace = true;
            ui->mygl->m_selectHalfEdge = false;
            ui->mygl->m_selectVertex = false;
            ui->mygl->m_faceDisplay.updateFace(face);
            ui->mygl->update();
        }
}

void MainWindow::slot_addVertexToSelectedHalfEdge(){
        HalfEdge *he1 = ui->mygl->m_selectedHalfEdgePtr;

        //if half edge is selected
        if(he1){
            HalfEdge *he2 = he1->getSym();
            //Insert a new vertex between V1 and V2

            // Get the start and end vertices of the current HalfEdge
            Vertex* v1 = he1->getVertex();
            Vertex* v2 = he2->getVertex();

            //compute midpoint
            glm::vec3 midpoint = (v1->getPosition() + v2->getPosition()) * 0.5f;

            //create new vertex at midpoint
            auto v3 = std::make_unique<Vertex>(midpoint, nullptr);

            // Create two new HalfEdges
            auto he1B = std::make_unique<HalfEdge>();
            auto he2B = std::make_unique<HalfEdge>();

            // set vertex pointers for newly created faces
            he1B->setVertex(v1);
            he2B->setVertex(v2);

            //set face pointers for newly created faces
            he1B->setFace(he1->getFace());
            he2B->setFace(he2->getFace());

            //set next half edge pointers
            he1B->setNext(he1->getNext());
            he2B->setNext(he2->getNext());
            he1->setNext(he1B.get());
            he2->setNext(he2B.get());

            //Adjust the sym, next, and vert pointers of HE1, HE2, HE1B, and HE2B so the data structure flows as it did before
            he1->setVertex(v3.get());
            he2->setVertex(v3.get());

            he1->setSym(he2B.get());
            he2B->setSym(he1);

            he2->setSym(he1B.get());
            he1B->setSym(he2);

            //add to GUI
            ui->vertsListWidget->addItem(QString::number(v3->getId()));

            ui->halfEdgesListWidget->addItem(QString::number(he1B->getId()));
            ui->halfEdgesListWidget->addItem(QString::number(he2B->getId()));

            //add newly created halfedges
            ui->mygl->m_mesh.halfEdges.push_back(std::move(he1B));
            ui->mygl->m_mesh.halfEdges.push_back(std::move(he2B));

            //add newly created midpoint vertex
            ui->mygl->m_mesh.vertices.push_back(std::move(v3));


            //ui->mygl->m_mesh.create();
        }

}

//function should only triangulate quadrangles for now
void MainWindow::slot_triangulateSelectedFace(){
        Face *face1 = ui->mygl->m_selectedFacePtr;

        if(face1){

            //step 1
            HalfEdge* he0 = face1->getHalfEdge();
            auto heA = std::make_unique<HalfEdge>();
            auto heB = std::make_unique<HalfEdge>();

            heA->setVertex(he0->getVertex());
            heB->setVertex(he0->getNext()->getNext()->getVertex());

            heA->setSym(heB.get());
            heB->setSym(heA.get());

            //step 2
            auto face2 = std::make_unique<Face>(ui->mygl->generateRandomColor());

            heA->setFace(face2.get());
            he0->getNext()->setFace(face2.get());
            he0->getNext()->getNext()->setFace(face2.get());

            heB->setFace(face1);

            //set face2's arbitrary half-edge pointer as heA
            face2->setHalfEdge(heA.get());

            //step 3
            heB->setNext(he0->getNext()->getNext()->getNext());
            he0->getNext()->getNext()->setNext(heA.get());
            heA->setNext(he0->getNext());
            he0->setNext(heB.get());

            //add to GUI
            ui->halfEdgesListWidget->addItem(QString::number(heA->getId()));
            ui->halfEdgesListWidget->addItem(QString::number(heB->getId()));

            ui->facesListWidget->addItem(QString::number(face2->getId()));

            //add newly created halfedges
            ui->mygl->m_mesh.halfEdges.push_back(std::move(heA));
            ui->mygl->m_mesh.halfEdges.push_back(std::move(heB));
            //add newly created face
            ui->mygl->m_mesh.faces.push_back(std::move(face2));


            //ui->mygl->create();
            //ui->mygl->update();
        }
}

void MainWindow::slot_modifyFaceColorRed(double value){
        Face *face = ui->mygl->m_selectedFacePtr;

        if(face){
               face->setColor(glm::vec3(value, face->getColor().g, face->getColor().b));
               ui->mygl->m_mesh.create();
               ui->mygl->update();
            }
        }
void MainWindow::slot_modifyFaceColorGreen(double value){
            Face *face = ui->mygl->m_selectedFacePtr;

            if(face){
               face->setColor(glm::vec3(face->getColor().r, value, face->getColor().b));
               ui->mygl->m_mesh.create();
               ui->mygl->update();
            }
}

void MainWindow::slot_modifyFaceColorBlue(double value){
            Face *face = ui->mygl->m_selectedFacePtr;

            if(face){
               face->setColor(glm::vec3(face->getColor().r, face->getColor().g, value));
               ui->mygl->m_mesh.create();
               ui->mygl->update();
            }
}


void MainWindow::slot_modifyVertexPositionX(double value){
            Vertex *vertex = ui->mygl->m_selectedVertexPtr;

            if(vertex){
               vertex->setPosition(glm::vec3(value, vertex->getPosition().y, vertex->getPosition().z));
               ui->mygl->m_vertDisplay.updateVertex(vertex);
               ui->mygl->m_mesh.create();
               ui->mygl->update();
            }
}

void MainWindow::slot_modifyVertexPositionY(double value){
            Vertex *vertex = ui->mygl->m_selectedVertexPtr;

            if(vertex){
               vertex->setPosition(glm::vec3(vertex->getPosition().x, value, vertex->getPosition().z));

               ui->mygl->m_vertDisplay.updateVertex(vertex);
               ui->mygl->m_mesh.create();
               ui->mygl->update();
            }
}

void MainWindow::slot_modifyVertexPositionZ(double value){
            Vertex *vertex = ui->mygl->m_selectedVertexPtr;

            if(vertex){
               vertex->setPosition(glm::vec3(vertex->getPosition().x, vertex->getPosition().y, value));

               ui->mygl->m_vertDisplay.updateVertex(vertex);
               ui->mygl->m_mesh.create();
               ui->mygl->update();
            }
}

void MainWindow::slot_ccSubdivide(){

            // ** COMPUTE CENTROIDS **
            std::map<Face*, Vertex*> faceCentroids;

            for (auto &face : ui->mygl->m_mesh.faces){
               glm::vec3 centroidPos = glm::vec3(0);
               int numVertices = 0;

               HalfEdge* start_halfEdge = face->getHalfEdge();
               HalfEdge* current_halfEdge = start_halfEdge;

               do {
                centroidPos += current_halfEdge->getVertex()->getPosition();
                numVertices++;
                current_halfEdge = current_halfEdge->getNext();
               } while (current_halfEdge != start_halfEdge);

               centroidPos /= static_cast<float>(numVertices);

               auto centroid = std::make_unique<Vertex>(centroidPos, nullptr);

               faceCentroids[face.get()] = centroid.get();

            }
            //** ADD MIDPOINT VERTICES **
            std::set<HalfEdge*> visitedEdges;
            //int fullEdges = ui->mygl->m_mesh.halfEdges.size()/2;

            int vertices_size = ui->mygl->m_mesh.halfEdges.size();

            for(int i = 0; i < vertices_size; i++){

               HalfEdge *he1 = ui->mygl->m_mesh.halfEdges[i].get();
               HalfEdge *he2 = he1->getSym();

               //make sure if we haven't dealt with the half edge already
               if (visitedEdges.find(he1) == visitedEdges.end()) {

                visitedEdges.insert(he1);
                visitedEdges.insert(he2);

                glm::vec3 midPoint = glm::vec3(0.0f);

               Vertex* v1 = he1->getVertex();
               Vertex* v2 = he2->getVertex();

                midPoint = v1->getPosition() + v2->getPosition();
                midPoint = midPoint + faceCentroids[he1->getFace()]->getPosition() + faceCentroids[he1->getSym()->getFace()]->getPosition();
                midPoint /= 4.0f;

               //compute midPoint vertex
               auto v3 = std::make_unique<Vertex>(midPoint, nullptr);

               //**Similar Code to Add Vertex Between Two HalfEdges

               //Insert a new vertex between V1 and V2

               //create new vertex at midpoint

               // Create two new HalfEdges
               auto he1B = std::make_unique<HalfEdge>();
               auto he2B = std::make_unique<HalfEdge>();

               // set vertex pointers for newly created faces
               he1B->setVertex(v1);
               he2B->setVertex(v2);

               //set face pointers for newly created faces
               he1B->setFace(he1->getFace());
               he2B->setFace(he2->getFace());

               //set next half edge pointers
               he1B->setNext(he1->getNext());
               he2B->setNext(he2->getNext());
               he1->setNext(he1B.get());
               he2->setNext(he2B.get());

               //Adjust the sym, next, and vert pointers of HE1, HE2, HE1B, and HE2B so the data structure flows as it did before
               he1->setVertex(v3.get());
               he2->setVertex(v3.get());

               he1->setSym(he2B.get());
               he2B->setSym(he1);

               he2->setSym(he1B.get());
               he1B->setSym(he2);

               //add midpoint to list of vertices
               ui->vertsListWidget->addItem(QString::number(v3->getId()));
               ui->mygl->m_mesh.vertices.push_back(std::move(v3));

               //add newly created halfedges to list of half edges
               ui->halfEdgesListWidget->addItem(QString::number(he1B->getId()));
               ui->halfEdgesListWidget->addItem(QString::number(he2B->getId()));

               ui->mygl->m_mesh.halfEdges.push_back(std::move(he1B));
               ui->mygl->m_mesh.halfEdges.push_back(std::move(he2B));

            }
            }

            //**SMOOTHEN VERTICES**//

            //for loop based on original vertices vector size since we're adding new midpoint vertices
            for (int i = 0; i< (int) ui->mygl->m_mesh.vertices.size(); i++){

                Vertex* v = ui->mygl->m_mesh.vertices[i].get();

                //sum of all adjacent midpoints to v
                glm::vec3 sumE = glm::vec3(0.0f);

                //sum of all centroids of all faces incident to v
                glm::vec3 sumF = glm::vec3(0.0f);

                //number of adjacent midpoints
                float n = 0;

                //terminating condition
                HalfEdge* start_edge = v->getHalfEdge();
                HalfEdge* current_edge = v->getHalfEdge();

                do {
                    //sumE:
                    //go to adjacent midpoint
                    //add midpoints position
                    sumE += current_edge->getVertex()->getPosition();
                    n++;

                    //sumF:
                    //get adjacent face
                    sumF += faceCentroids.find(current_edge->getFace())->second->getPosition();

                    //reverse and go towards next midpoint
                    current_edge = current_edge->getSym()->getNext();

                } while(current_edge != start_edge);

                //calculate smoothened position
                glm::vec3 smoothenedPosition = ((n-2) * v->getPosition()) / n + sumE / (n * n) + sumF / (n * n) ;

                //set vertex to smoothened position
                v->setPosition(smoothenedPosition);
               }

               //**QUADRANGULATE**//

               //step 4: for each original face, split that face into N quadrangle faces
               //Writing a separate function to "quadrangulate" a face given the new vertices may be helpful.

               //for loop based on original face vector size since we're adding quad faces
               int facesSize = ui->mygl->m_mesh.faces.size();

               for(int i = 0; i < facesSize; i++){

                Face *face = ui->mygl->m_mesh.faces[i].get();

                HalfEdge * start_edge = face->getHalfEdge();
                HalfEdge * current_edge = start_edge;

                //first quadrangle
                //face of first quad will just be the original face
                auto heToCentroid_q1 = std::make_unique<HalfEdge>();
                auto heAwayFromCentroid_q1 = std::make_unique<HalfEdge>();

                heToCentroid_q1->setVertex(faceCentroids[face]);
                heToCentroid_q1->setNext(heAwayFromCentroid_q1.get());
                heToCentroid_q1->setFace(face);

                heAwayFromCentroid_q1->setFace(face);
                heAwayFromCentroid_q1->setNext(current_edge);
                heAwayFromCentroid_q1->setVertex(current_edge->getSym()->getVertex());

                //use this pointer to set up symmetries and close loop
                HalfEdge * previous_quad = current_edge;

                //move current edge to the start of the next quad i.e. the next sub surface
                current_edge = current_edge->getNext()->getNext();

                //close half edge loop within quad
                previous_quad->getNext()->setNext(heToCentroid_q1.get());

                //push to mesh + widgets:
                //add newly created halfedges
                ui->halfEdgesListWidget->addItem(QString::number(heToCentroid_q1->getId()));
                ui->halfEdgesListWidget->addItem(QString::number(heAwayFromCentroid_q1->getId()));

                ui->mygl->m_mesh.halfEdges.push_back(std::move(heToCentroid_q1));

                HalfEdge* raw_heToCentroid_q1 = ui->mygl->m_mesh.halfEdges.back().get();

                ui->mygl->m_mesh.halfEdges.push_back(std::move(heAwayFromCentroid_q1));

                HalfEdge* raw_heAwayFromCentroid_q1 = ui->mygl->m_mesh.halfEdges.back().get();

                auto centroidVertex = std::make_unique<Vertex>(faceCentroids[face]->getPosition(), nullptr);
                do{
                    //quadface

                    auto quadFace = std::make_unique<Face>(ui->mygl->generateRandomColor());
                    auto heToCentroid = std::make_unique<HalfEdge>();
                    auto heAwayFromCentroid = std::make_unique<HalfEdge>();

                    heToCentroid->setVertex(centroidVertex.get());
                    heToCentroid->setNext(heAwayFromCentroid.get());
                    heToCentroid->setFace(quadFace.get());

                    heAwayFromCentroid->setFace(quadFace.get());
                    heAwayFromCentroid->setNext(current_edge);
                    heAwayFromCentroid->setVertex(current_edge->getSym()->getVertex());

                    quadFace->setHalfEdge(current_edge);

                    //set up symmetries

                        //close quad loop within previous quad
                    previous_quad->getNext()->setNext(heToCentroid.get());
                        //set sym mutually
                    heAwayFromCentroid->setSym(previous_quad->getNext()->getNext());
                    previous_quad->getNext()->getNext()->setSym(heAwayFromCentroid.get());

                    previous_quad = current_edge;
                    current_edge =current_edge->getNext()->getNext();

                    //push stuff to mesh:
                    //add newly created halfedges
                    ui->halfEdgesListWidget->addItem(QString::number(heToCentroid->getId()));
                    ui->halfEdgesListWidget->addItem(QString::number(heAwayFromCentroid->getId()));

                    ui->mygl->m_mesh.halfEdges.push_back(std::move(heToCentroid));
                    ui->mygl->m_mesh.halfEdges.push_back(std::move(heAwayFromCentroid));

                    //add quad face
                    ui->facesListWidget->addItem(QString::number(quadFace->getId()));
                    ui->mygl->m_mesh.faces.push_back(std::move(quadFace));

                } while(current_edge != start_edge);

                //set up symmetry of the first quadrangle's half edge away from centroid to previous quad's half edge to centroid
                raw_heAwayFromCentroid_q1->setSym(previous_quad->getNext()->getNext());
                previous_quad->getNext()->getNext()->setSym(raw_heAwayFromCentroid_q1);

                ui->vertsListWidget->addItem(QString::number(centroidVertex->getId()));
                ui->mygl->m_mesh.vertices.push_back(std::move(centroidVertex));

                }
               ui->mygl->m_mesh.create();
               ui->mygl->update();
               }

void MainWindow::slot_loadSkeleton(){
               //open file
               QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString("../../../../jsons"), tr("JSON Files (*.json)"));
               if(!fileName.isEmpty()) {
                ui->mygl->loadJSONFile(fileName);
               }
}





