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
        //QString fileName = QFileDialog::getOpenFileName(this, "Open OBJ File", "", "OBJ Files (*.obj)");
        if(!fileName.isEmpty()) {
            ui->mygl->slot_loadOBJFile(fileName);
}
}


