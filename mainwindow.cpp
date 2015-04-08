#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "graph_scene.h"
#include "qgraphics_ellipse_node.h"
#include "qgraphics_simple_line_edge.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    _graphScene = new DirectedGraphScene<QGraphicsEllipseNode, QGraphicsSimpleLineEdge>();
    _ui->graphicsView->setGraphScene(_graphScene);
}

MainWindow::~MainWindow()
{
    delete _ui;
    delete _graphScene;
}

void MainWindow::on_actionPointer_toggled(bool checked)
{
    if (checked)
    {
        _ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
        _ui->actionPencil->setChecked(false);
    }
    else
        _ui->actionPointer->setChecked(true);
}

void MainWindow::on_actionPencil_toggled(bool checked)
{
    if (checked)
    {
        _ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
        _ui->actionPointer->setChecked(false);
    }
    else
        _ui->actionPencil->setChecked(true);
}
