#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QtWidgets/qlistwidget.h"
#include "QtWidgets/qtreewidget.h"
#include <QMainWindow>



namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_actionQuit_triggered();

    void on_actionCamera_Controls_triggered();

    //**process obj file
    void slot_openFile();

    //**GUI
    void slot_vertexSelected(QListWidgetItem *item);
    void slot_halfEdgeSelected(QListWidgetItem *item);
    void slot_faceSelected(QListWidgetItem *item);

    //**hw06: simple mesh topology operations
    void slot_addVertexToSelectedHalfEdge();
    void slot_triangulateSelectedFace();

    //modifying vertex and face attributes
    void slot_modifyFaceColorRed(double value);
    void slot_modifyFaceColorBlue(double value);
    void slot_modifyFaceColorGreen(double value);

    void slot_modifyVertexPositionX(double value);
    void slot_modifyVertexPositionY(double value);
    void slot_modifyVertexPositionZ(double value);

    //slot for catmull clark subdivision
    void slot_ccSubdivide();

    //for each original face, split face into N quadrangle faces
    //void quadrangulateFace(Face* f, std::map<HalfEdges*, glm::vec3>& edgeVertices, const glm::vec3& faceCentroid);

    //**hw07: load skeleton
    void slot_loadSkeleton();

    //Interactive Skeleton
    void slot_addRootToTreeWidget(QTreeWidgetItem *);
    void slot_selectJoint(QTreeWidgetItem*);

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
