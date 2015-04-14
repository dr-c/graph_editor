#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "graph_scene.h"
#include "graph_scene_mode.h"

#include "qgraphics_ellipse_node.h"
#include "qgraphics_rounded_rect_node.h"

#include "qgraphics_simple_line_edge.h"
#include "qgraphics_cubic_arrow_edge.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    _directedGraph = new DirectedGraph<NodeInfo, EdgeInfo>();
    _undirectedGraph = new UndirectedGraph<NodeInfo, EdgeInfo>();
    //_graphScene = new UndirectedGraphScene<QGraphicsRoundedRectNode, QGraphicsSimpleLineEdge>(_undirectedGraph, new PointerMode());
    _graphScene = new DirectedGraphScene<QGraphicsEllipseNode, QGraphicsCubicArrowEdge>(_directedGraph, new PointerMode());
    connect(_graphScene->history(), SIGNAL(newItemAdded()), this, SLOT(updateUndoRedo()));

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

void MainWindow::on_actionUndo_triggered()
{
    if (!_graphScene->history()->undo())
        _ui->actionUndo->setEnabled(false);
    _ui->actionRedo->setEnabled(true);
}

void MainWindow::on_actionRedo_triggered()
{
    if (!_graphScene->history()->redo())
        _ui->actionRedo->setEnabled(false);
    _ui->actionUndo->setEnabled(true);
}

void MainWindow::updateUndoRedo()
{
    _ui->actionUndo->setEnabled(true);
    _ui->actionRedo->setEnabled(false);
}
