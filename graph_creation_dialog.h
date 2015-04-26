/*
 * GraphCreationDialog - Dialog for determination Graph type, and parameters customization
 *  for graphical representation, such as shape of Nodes/Edges, brushes, pens, fonts, etc.
 *
 * configureGraphScene(Scene) sets graphical parameters for scene items.
 *
 * GraphCreationDialog uses BrushChooserFrame and PenSchooserFrame.
 *
 * <TODO>: choise of shape.
 */

#ifndef GRAPH_CREATION_DIALOG_H
#define GRAPH_CREATION_DIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include <memory>

class GraphScene;
class GraphSceneMode;
class GraphConfiguration;
class GraphicsNodeCreator;
class GraphicsEdgeCreator;

namespace Ui {
class GraphCreationDialog;
}

class GraphCreationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GraphCreationDialog(QWidget *parent = 0);
    ~GraphCreationDialog();

    void setDirected(bool isDirected);
    bool isDirected() const;
    void lockGraphTypeButtons();
    void unLockGraphTypeButtons();
    void setConfiguration(const std::shared_ptr<GraphConfiguration> &config);
    std::shared_ptr<GraphConfiguration> getConfiguration() const;

public slots:
    void reset();

private slots:
    void on_nodeFontButton_clicked();
    void on_edgeFontButton_clicked();

private:
    void changeFont(QPushButton *button);
    GraphicsNodeCreator *getNodeCreator() const;
    GraphicsEdgeCreator *getEdgeCreator() const;

    Ui::GraphCreationDialog *_ui;
    QButtonGroup _graphTypeGroup;
    QButtonGroup _shapeNodesGroup;
    QButtonGroup _formEdgesGroup;
};

#endif // GRAPH_CREATION_DIALOG_H
