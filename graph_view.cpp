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
    return dynamic_cast<BasicGraphScene*>(QGraphicsView::scene());
}
