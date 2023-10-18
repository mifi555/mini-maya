#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "cameracontrolshelp.h"

#include <QFileDialog>



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
            ui->mygl->m_selectFace = true;
            ui->mygl->m_selectHalfEdge = false;
            ui->mygl->m_selectVertex = false;
            ui->mygl->m_faceDisplay.updateFace(face);
            ui->mygl->update();
        }
}



