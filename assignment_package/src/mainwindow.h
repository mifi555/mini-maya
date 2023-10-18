#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QtWidgets/qlistwidget.h"
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

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
