#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "graph_scene.h"
#include "graph_scene_mode.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow),
    _tabBar(this),
    _creationDialog(this),
    _curScene(nullptr),
    _tabId(0)
{
    _ui->setupUi(this);

    _tabBar.setTabsClosable(true);
    _ui->verticalLayout->insertWidget(0, &_tabBar);

    connect(_ui->graphicsView, SIGNAL(blankSceneMousePressed()), this, SLOT(on_actionNewGraph_triggered()));
    connect(&_tabBar, SIGNAL(currentChanged(int)), this, SLOT(changeTab(int)));
    connect(&_tabBar, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
}

MainWindow::~MainWindow()
{
    _tabId = -1;
    while (_tabBar.count() > 0)
        closeTab(0);
    delete _ui;
}

void MainWindow::on_actionPointer_triggered(bool checked)
{
    if (checked)
    {
        _ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
        if (_curScene != nullptr && _curScene->mode()->type() != PointerMode::Type)
            _curScene->setMode(static_cast<std::shared_ptr<GraphSceneMode>>(std::make_shared<PointerMode>(_curScene)));
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
        if (_curScene != nullptr && _curScene->mode()->type() != PencilMode::Type)
            _curScene->setMode(static_cast<std::shared_ptr<GraphSceneMode>>(std::make_shared<PencilMode>(_curScene)));
        _ui->actionPointer->setChecked(false);
    }
    else
        _ui->actionPencil->setChecked(true);
}

void MainWindow::on_actionUndo_triggered()
{
    assert(_curScene != nullptr);
    if (!_curScene->history().undo())
        _ui->actionUndo->setEnabled(false);
    _ui->actionRedo->setEnabled(true);
}

void MainWindow::on_actionRedo_triggered()
{
    assert(_curScene != nullptr);
    if (!_curScene->history().redo())
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
    _creationDialog.setDirected(true);
    _creationDialog.reset();
    if (_creationDialog.exec() == QDialog::Accepted)
    {
        auto mode = (_ui->actionPencil->isChecked()) ?
                    static_cast<std::shared_ptr<GraphSceneMode>>(std::make_shared<PencilMode>()) :
                    static_cast<std::shared_ptr<GraphSceneMode>>(std::make_shared<PointerMode>());

        auto graph = _creationDialog.isDirected() ?
                    static_cast<std::shared_ptr<WeightedGraph>>(std::make_shared<DirectedGraph<NodeInfo, EdgeInfo>>()) :
                    static_cast<std::shared_ptr<WeightedGraph>>(std::make_shared<UndirectedGraph<NodeInfo, EdgeInfo>>());

        GraphScene *scene = new GraphScene(std::move(graph), std::move(_creationDialog.getConfiguration()), std::move(mode));

        _curScene = scene;
        _ui->graphicsView->setGraphScene(scene);
        int index = _tabBar.addTab(QString::number(++_tabId));
        _tabBar.setTabData(index, QVariant::fromValue(scene));
        _tabBar.setCurrentIndex(index);
        _ui->actionConfigure->setEnabled(true);

        connect(&scene->history(), SIGNAL(newItemAdded()), this, SLOT(updateUndoRedo()));
    }
}

void MainWindow::changeTab(int index)
{
    QVariant data = _tabBar.tabData(index);
    if (index != -1 && data != QVariant() && _tabId != -1)
    {
        _curScene = data.value<GraphScene*>();
        _ui->graphicsView->setGraphScene(_curScene);
        _ui->actionUndo->setEnabled(_curScene->history().canUndo());
        _ui->actionRedo->setEnabled(_curScene->history().canRedo());
        if (_ui->actionPencil->isChecked())
            on_actionPencil_triggered(true);
        else
            on_actionPointer_triggered(true);
    }
    else
    {
        _ui->actionUndo->setEnabled(false);
        _ui->actionRedo->setEnabled(false);
        _ui->actionConfigure->setEnabled(false);
    }
}

void MainWindow::closeTab(int index)
{
    delete _tabBar.tabData(index).value<GraphScene*>();
    _tabBar.removeTab(index);
}

void MainWindow::on_actionConfigure_triggered()
{
    _creationDialog.setDirected(_curScene->graph()->isDirected());
    _creationDialog.lockGraphTypeButtons();
    _creationDialog.setConfiguration(_curScene->config());
    if (_creationDialog.exec() == QDialog::Accepted)
        _curScene->setConfig(_creationDialog.getConfiguration());
    _creationDialog.unLockGraphTypeButtons();
}
