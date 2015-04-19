#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "graph_scene.h"
#include "graph_scene_mode.h"

#include "graph_creation_dialog.h"

#include "qgraphics_ellipse_node.h"
#include "qgraphics_rounded_rect_node.h"

#include "qgraphics_simple_line_edge.h"
#include "qgraphics_cubic_arrow_edge.h"

#include <QTabBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow),
    _creationDialog(new GraphCreationDialog(this)),
    _graphScene(nullptr),
    _tabId(0)
{
    _ui->setupUi(this);

    _tabBar = new QTabBar(this);
    _tabBar->setTabsClosable(true);
    _ui->verticalLayout->insertWidget(0, _tabBar);

    connect(_ui->graphicsView, SIGNAL(blankSceneMousePressed()), this, SLOT(on_actionNewGraph_triggered()));
    connect(_tabBar, SIGNAL(currentChanged(int)), this, SLOT(changeTab(int)));
    connect(_tabBar, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
}

MainWindow::~MainWindow()
{
    _tabId = -1;
    while (_tabBar->count() > 0)
        closeTab(0);
    delete _tabBar;
    delete _creationDialog;
    delete _ui;
}

void MainWindow::on_actionPointer_triggered(bool checked)
{
    if (checked)
    {
        _ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
        if (_graphScene != nullptr && _graphScene->mode()->type() != PointerMode::Type)
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
        if (_graphScene != nullptr && _graphScene->mode()->type() != PencilMode::Type)
            _graphScene->setMode(new PencilMode(_graphScene));
        _ui->actionPointer->setChecked(false);
    }
    else
        _ui->actionPencil->setChecked(true);
}

void MainWindow::on_actionUndo_triggered()
{
    assert(_graphScene != nullptr);
    if (!_graphScene->history()->undo())
        _ui->actionUndo->setEnabled(false);
    _ui->actionRedo->setEnabled(true);
}

void MainWindow::on_actionRedo_triggered()
{
    assert(_graphScene != nullptr);
    if (!_graphScene->history()->redo())
        _ui->actionRedo->setEnabled(false);
    _ui->actionUndo->setEnabled(true);
}

void MainWindow::updateUndoRedo()
{
    _ui->actionUndo->setEnabled(true);
    _ui->actionRedo->setEnabled(false);
}

void MainWindow::on_actionNewGraph_triggered()
{
    if (_creationDialog->exec() == QDialog::Accepted)
    {
        GraphSceneMode *mode = nullptr;
        if (_ui->actionPencil->isChecked())
            mode = new PencilMode();
        else
            mode = new PointerMode();

        BasicGraphScene *scene = nullptr;
        if (_creationDialog->isDirected())
            scene = new DirectedGraphScene<QGraphicsEllipseNode, QGraphicsCubicArrowEdge>(
                        std::make_shared<DirectedGraph<NodeInfo, EdgeInfo>>(), mode);
        else
            scene = new UndirectedGraphScene<QGraphicsRoundedRectNode, QGraphicsSimpleLineEdge>(
                        std::make_shared<UndirectedGraph<NodeInfo, EdgeInfo>>(), mode);
        _graphScene = scene;
        _creationDialog->configureGraphScene(scene);
        _ui->graphicsView->setGraphScene(scene);
        int index = _tabBar->addTab(QString::number(++_tabId));
        _tabBar->setTabData(index, QVariant::fromValue(scene));
        _tabBar->setCurrentIndex(index);

        connect(scene->history(), SIGNAL(newItemAdded()), this, SLOT(updateUndoRedo()));
    }
}

void MainWindow::changeTab(int index)
{
    QVariant data = _tabBar->tabData(index);
    if (index != -1 && data != QVariant() && _tabId != -1)
    {
        _graphScene = data.value<BasicGraphScene*>();
        _ui->graphicsView->setGraphScene(_graphScene);
        _ui->actionUndo->setEnabled(_graphScene->history()->canUndo());
        _ui->actionRedo->setEnabled(_graphScene->history()->canRedo());
        if (_ui->actionPencil->isChecked())
            on_actionPencil_triggered(true);
        else
            on_actionPointer_triggered(true);
    }
    else
    {
        _ui->actionUndo->setEnabled(false);
        _ui->actionRedo->setEnabled(false);
    }
}

void MainWindow::closeTab(int index)
{
    delete _tabBar->tabData(index).value<BasicGraphScene*>();
    _tabBar->removeTab(index);
}
