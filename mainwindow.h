#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "item_info.h"

namespace Ui {
class MainWindow;
}

class BasicGraphScene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionPointer_triggered(bool checked);
    void on_actionPencil_triggered(bool checked);

private:
    Ui::MainWindow                      *_ui;
    DirectedGraph<NodeInfo, EdgeInfo>   *_directedGraph;
    BasicGraphScene                     *_graphScene;
};

#endif // MAINWINDOW_H
