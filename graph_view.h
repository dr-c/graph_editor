/*
 * The GraphView class provides a widget for displaying the contents of a BasicGraphScene.
 *
 * GraphView is wrapper on QGraphicsView, which provides easy access to BasicGraphScene.
 */
#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QGraphicsView>

class BasicGraphScene;

class GraphView : public QGraphicsView
{
public:
    GraphView(QWidget *parent = 0);
    GraphView(BasicGraphScene *graph, QWidget *parent = 0);
    virtual ~GraphView() override = default;

    void setGraphScene(BasicGraphScene *graph);
    BasicGraphScene *graphScene() const;
};

#endif // GRAPHVIEW_H
