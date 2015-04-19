#include "graph_view.h"
#include "graph_scene.h"

GraphView::GraphView(QWidget *parent)
    : QGraphicsView(parent)
{

}

GraphView::GraphView(BasicGraphScene *graph, QWidget *parent)
    : QGraphicsView(graph, parent)
{

}

void GraphView::setGraphScene(BasicGraphScene *graph)
{
    QGraphicsView::setScene(graph);
}

BasicGraphScene* GraphView::graphScene() const
{
    assert(dynamic_cast<BasicGraphScene*>(QGraphicsView::scene()) != nullptr);
    return static_cast<BasicGraphScene*>(QGraphicsView::scene());
}

void GraphView::mousePressEvent(QMouseEvent *event)
{
    if (scene() == nullptr)
        emit blankSceneMousePressed();
    QGraphicsView::mousePressEvent(event);
}
