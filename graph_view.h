#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QGraphicsView>

class GraphScene;

class GraphView : public QGraphicsView
{
public:
    GraphView(QWidget *parent = 0);
    GraphView(GraphScene *graph, QWidget *parent = 0);

    void setGraphScene(GraphScene *graph);
    GraphScene *graphScene() const;
};

#endif // GRAPHVIEW_H
