#include "graph_scene.h"

BasicGraphScene::BasicGraphScene(Graph<NodeInfo, EdgeInfo> *graph, QObject *parent)
    : QGraphicsScene(parent), _graph(graph)
{

}

BasicGraphScene::~BasicGraphScene()
{
    delete _graph;
}

void BasicGraphScene::addNode(const QPointF &centerPos, int weight)
{
    WeightedNode *node = _graph->createNode();
    node.setPos(centerPos);
    node.setWeight(weight);
    addItem(createGraphicsNode(node));
}

void BasicGraphScene::addEdge(int weight)
{
    WeightedEdge *edge = _graph->createEdge();
    edge.setWeight(weight);
    addItem(createGraphicsEdge(edge));
}

void BasicGraphScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

void BasicGraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void BasicGraphScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void BasicGraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void BasicGraphScene::keyPressEvent(QKeyEvent *keyEvent)
{
    QGraphicsScene::keyPressEvent(keyEvent);
}
