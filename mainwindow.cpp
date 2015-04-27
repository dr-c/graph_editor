#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "graph_scene.h"
#include "graph_scene_mode.h"

#include "file_graph_manager.h"

#include <QFileDialog>
#include <QMessageBox>

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
        auto graph = _creationDialog.isDirected() ?
                    static_cast<std::shared_ptr<WeightedGraph>>(std::make_shared<DirectedGraph<NodeInfo, EdgeInfo>>()) :
                    static_cast<std::shared_ptr<WeightedGraph>>(std::make_shared<UndirectedGraph<NodeInfo, EdgeInfo>>());

        addNewScene(graph, _creationDialog.getConfiguration());
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
        _ui->actionSaveAs->setEnabled(false);
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

void MainWindow::on_actionSave_triggered()
{

}

void MainWindow::on_actionSaveAs_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save Graph as ...", ".",
                                                    "Graph files (*.gxml);;XML files (*.xml);;All files (*.*)");
    if (fileName.isEmpty())
        return;

    if (!fileName.endsWith(".gxml", Qt::CaseInsensitive) && !fileName.endsWith(".xml", Qt::CaseInsensitive))
        fileName += ".gxml";

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        FileGraphManager::save(file, _curScene);
        file.close();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Cann't write to file " + fileName);
        msgBox.exec();
    }
}

void MainWindow::on_actionLoad_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Graph...", ".",
                                                    "Graph files (*.gxml);;XML files (*.xml);;All files (*.*)");
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
    {
        std::shared_ptr<WeightedGraph> graph;
        std::shared_ptr<GraphConfiguration> config;
        if (FileGraphManager::load(file, graph, config))
            addNewScene(graph, config);
        else
        {
            QMessageBox msgBox;
            msgBox.setText("File " + fileName + " has incorrect format.");
            msgBox.exec();
        }
        file.close();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Cann't read file " + fileName);
        msgBox.exec();
    }
}

void MainWindow::addNewScene(std::shared_ptr<WeightedGraph> graph, std::shared_ptr<GraphConfiguration> config)
{
    auto mode = (_ui->actionPencil->isChecked()) ?
                static_cast<std::shared_ptr<GraphSceneMode>>(std::make_shared<PencilMode>()) :
                static_cast<std::shared_ptr<GraphSceneMode>>(std::make_shared<PointerMode>());

    _curScene = new GraphScene(std::move(graph), std::move(config), std::move(mode));
    _ui->graphicsView->setGraphScene(_curScene);
    int index = _tabBar.addTab(QString::number(++_tabId));
    _tabBar.setTabData(index, QVariant::fromValue(_curScene));
    _tabBar.setCurrentIndex(index);
    _ui->actionConfigure->setEnabled(true);
    _ui->actionSaveAs->setEnabled(true);

    connect(&_curScene->history(), SIGNAL(newItemAdded()), this, SLOT(updateUndoRedo()));
}
