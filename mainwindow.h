#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabBar>
#include <vector>
#include <memory>

#include "item_info.h"

#include "graph_creation_dialog.h"

namespace Ui {
class MainWindow;
}

class GraphScene;
class GraphSceneMode;
class QXmlStreamWriter;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionPointer_triggered(bool checked);
    void on_actionPencil_triggered(bool checked);
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void updateUndoRedo();
    void on_actionNewGraph_triggered();
    void changeTab(int index);
    void closeTab(int index);
    void on_actionConfigure_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_actionLoad_triggered();

private:
    int addNewScene(std::shared_ptr<WeightedGraph> graph, std::shared_ptr<GraphConfiguration> config);

    Ui::MainWindow      *_ui;
    QTabBar              _tabBar;
    GraphCreationDialog  _creationDialog;
    GraphScene          *_curScene;
    int                  _tabId;
};

#endif // MAINWINDOW_H
