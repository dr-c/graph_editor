#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>

#include "item_info.h"

namespace Ui {
class MainWindow;
}

class GraphScene;
class GraphCreationDialog;
class QTabBar;

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

private:
    Ui::MainWindow                 *_ui;
    QTabBar                        *_tabBar;
    GraphCreationDialog            *_creationDialog;
    GraphScene                *_graphScene;
    int                             _tabId;
};

#endif // MAINWINDOW_H
