#include "graph_scene.h"

#include "graph_scene_mode.h"
#include "qgraphics_node.h"
#include "qgraphics_edge.h"

#include <QGraphicsSceneMouseEvent>

BasicGraphScene::BasicGraphScene(WeightedGraph *graph, GraphSceneMode *mode, QObject *parent)
    : QGraphicsScene(parent), _graph(graph), _mode(mode)
{
    _mode->setScene(this);
}

BasicGraphScene::~BasicGraphScene()
{
    delete _graph;
}

void BasicGraphScene::addNode(const QPointF &centerPos, int weight)
{
    WeightedNode *node = _graph->createNode();
    node->setPos(centerPos);
    node->setWeight(weight);
    QGraphicsNode *graphics_node = createGraphicsNode(node);
    _mode->setItemFlags(graphics_node);
    addItem(graphics_node);
}

void BasicGraphScene::addEdge(int weight)
{
    WeightedEdge *edge = _graph->createEdge();
    edge->setWeight(weight);
    QGraphicsEdge *graphics_edge = createGraphicsEdge(edge);
    graphics_edge->setFlags(QGraphicsItem::ItemIsFocusable);
    addItem(graphics_edge);
}

const GraphSceneMode *BasicGraphScene::mode() const
{
    return _mode;
}

void BasicGraphScene::setMode(GraphSceneMode *mode)
{
    delete _mode;
    _mode = mode;
    for (auto pair : _graph->nodes())
        _mode->setItemFlags(pair.second->graphicsNode());
}

void BasicGraphScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton && itemAt(mouseEvent->scenePos(), QTransform()) == nullptr)
        addNode(mouseEvent->scenePos());

    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

void BasicGraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    _mode->mouseMoveEvent(mouseEvent);
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void BasicGraphScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    _mode->mousePressEvent(mouseEvent);
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void BasicGraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    _mode->mouseReleaseEvent(mouseEvent);
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void BasicGraphScene::keyPressEvent(QKeyEvent *keyEvent)
{
    _mode->keyPressEvent(keyEvent);
    QGraphicsScene::keyPressEvent(keyEvent);
}

void BasicGraphScene::keyReleaseEvent(QKeyEvent *keyEvent)
{
    _mode->keyReleaseEvent(keyEvent);
    QGraphicsScene::keyReleaseEvent(keyEvent);
}
