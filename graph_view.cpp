#include "graph_view.h"
#include "graph_scene.h"

GraphView::GraphView(QWidget *parent)
    : QGraphicsView(parent)
{

}

GraphView::GraphView(GraphScene *graph, QWidget *parent)
    : QGraphicsView(graph, parent)
{

}

void GraphView::setGraphScene(GraphScene *graph)
{
    QGraphicsView::setScene(graph);
}

GraphScene* GraphView::graphScene() const
{
    assert(dynamic_cast<GraphScene*>(QGraphicsView::scene()) != nullptr);
    return static_cast<GraphScene*>(QGraphicsView::scene());
}

void GraphView::mousePressEvent(QMouseEvent *event)
{
    if (scene() == nullptr)
        emit blankSceneMousePressed();
    QGraphicsView::mousePressEvent(event);
}
