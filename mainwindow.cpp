#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "graph_scene.h"
#include "graph_scene_mode.h"

#include "qgraphics_ellipse_node.h"
#include "qgraphics_rounded_rect_node.h"

#include "qgraphics_simple_line_edge.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    _directedGraph = new DirectedGraph<NodeInfo, EdgeInfo>();
    _graphScene = new DirectedGraphScene<QGraphicsRoundedRectNode, QGraphicsSimpleLineEdge>(_directedGraph, new PointerMode());

    _ui->graphicsView->setGraphScene(_graphScene);
}

MainWindow::~MainWindow()
{
    delete _ui;
    delete _graphScene;
    delete _directedGraph;
}

void MainWindow::on_actionPointer_triggered(bool checked)
{
    if (checked)
    {
        _ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
        if (_graphScene->mode()->type() != PointerMode::Type)
            _graphScene->setMode(new PointerMode(_graphScene));
        _ui->actionPencil->setChecked(false);
    }
    else
        _ui->actionPointer->setChecked(true);
}

void MainWindow::on_actionPencil_triggered(bool checked)
{
    if (checked)
    {
        _ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
        if (_graphScene->mode()->type() != PencilMode::Type)
            _graphScene->setMode(new PencilMode(_graphScene));
        _ui->actionPointer->setChecked(false);
    }
    else
        _ui->actionPencil->setChecked(true);
}
