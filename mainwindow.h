#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_actionPointer_toggled(bool checked);
    void on_actionPencil_toggled(bool checked);

private:
    Ui::MainWindow *_ui;

    BasicGraphScene *_graphScene;
};

#endif // MAINWINDOW_H
