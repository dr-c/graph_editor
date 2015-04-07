#include "graph_scene.h"

GraphScene::GraphScene(Graph<NodeInfo, EdgeInfo> *graph, QObject *parent)
    : QGraphicsScene(parent), _graph(graph)
{

}

GraphScene::~GraphScene()
{
    delete _graph;
}

void GraphScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void GraphScene::keyPressEvent(QKeyEvent *keyEvent)
{
    QGraphicsScene::keyPressEvent(keyEvent);
}


DirectedGraphScene::DirectedGraphScene(QObject *parent)
    : GraphScene(new DirectedGraph<NodeInfo, EdgeInfo>(), parent)
{

}

DirectedGraphScene::~DirectedGraphScene()
{

}

UndirectedGraphScene::UndirectedGraphScene(QObject *parent)
    : GraphScene(new UndirectedGraph<NodeInfo, EdgeInfo>(), parent)
{

}

UndirectedGraphScene::~UndirectedGraphScene()
{

}
