#ifndef GRAPH_CREATION_DIALOG_H
#define GRAPH_CREATION_DIALOG_H

#include <QDialog>

class QButtonGroup;
class BasicGraphScene;
class GraphSceneMode;

namespace Ui {
class GraphCreationDialog;
}

class GraphCreationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GraphCreationDialog(QWidget *parent = 0);
    ~GraphCreationDialog();

    bool isDirected() const;
    void configureGraphScene(BasicGraphScene *scene) const;

private slots:
    void init();
    void on_nodeFontButton_clicked();
    void on_edgeFontButton_clicked();

private:
    void changeFont(QPushButton *button);

    Ui::GraphCreationDialog *_ui;
    QButtonGroup *_graphTypeGroup;
    QButtonGroup *_shapeNodesGroup;
    QButtonGroup *_formEdgesGroup;
};

#endif // GRAPH_CREATION_DIALOG_H
