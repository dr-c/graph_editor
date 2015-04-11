#include "graph_scene.h"

#include "graph_scene_mode.h"

#include <QGraphicsSceneMouseEvent>

BasicGraphScene::BasicGraphScene(WeightedGraph *graph, GraphSceneMode *mode, QObject *parent)
    : QGraphicsScene(parent), _graph(graph), _mode(mode),
      _nodePen(QPen(QColor(Qt::blue), 2)),
      _nodeHoverPen(QPen(QColor(Qt::red), 2)),
      _itemFont(QFont("Times New Roman", 10)),
      _nodeBrush(QBrush(QColor(192, 192, 192, 192))),
      _edgeBrush(QBrush(QColor(192, 192, 192, 255)))
{
    _mode->setScene(this);
}

BasicGraphScene::~BasicGraphScene()
{
    delete _mode;
}

QGraphicsNode *BasicGraphScene::addNode(const QPointF &centerPos, int weight)
{
    WeightedNode *node = _graph->createNode();
    node->setPos(centerPos);
    node->setWeight(weight);
    return addNode(node);
}

QGraphicsNode *BasicGraphScene::addNode(WeightedNode *node)
{
    assert(node->graph() == this->_graph);
    QGraphicsNode *graphics_node = createGraphicsNode(node);
    graphics_node->setPen(_nodePen);
    graphics_node->setFont(_itemFont);
    graphics_node->setBrush(_nodeBrush);
    graphics_node->setHoverPen(_nodeHoverPen);
    graphics_node->setAcceptHoverEvents(true);
    _mode->setItemFlags(graphics_node);
    addItem(graphics_node);
    return graphics_node;
}

QGraphicsEdge *BasicGraphScene::addEdge(int weight)
{
    WeightedEdge *edge = _graph->createEdge();
    edge->setWeight(weight);
    return addEdge(edge);
}

QGraphicsEdge *BasicGraphScene::addEdge(WeightedEdge *edge)
{
    assert(edge->graph() == this->_graph);
    QGraphicsEdge *graphics_edge = createGraphicsEdge(edge);
    graphics_edge->setFont(_itemFont);
    graphics_edge->setBrush(_edgeBrush);
    graphics_edge->setFlags(QGraphicsItem::ItemIsFocusable);
    addItem(graphics_edge);
    return graphics_edge;
}

const WeightedGraph *BasicGraphScene::graph() const
{
    return _graph;
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

void BasicGraphScene::setNodePen(const QPen &pen)
{
    _nodePen = pen;
    for (auto pair : _graph->nodes())
        pair.second->graphicsNode()->setPen(_nodePen);
}

void BasicGraphScene::setNodeFont(const QFont &font)
{
    _itemFont = font;
    for (auto pair : _graph->nodes())
        pair.second->graphicsNode()->setFont(_itemFont);
}

void BasicGraphScene::setNodeBrush(const QBrush &brush)
{
    _nodeBrush = brush;
    for (auto pair : _graph->nodes())
        pair.second->graphicsNode()->setBrush(_nodeBrush);
}

QPen BasicGraphScene::nodePen() const
{
    return _nodePen;
}

QFont BasicGraphScene::nodeFont() const
{
    return _itemFont;
}

QBrush BasicGraphScene::nodeBrush() const
{
    return _nodeBrush;
}

void BasicGraphScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    _mode->mouseDoubleClickEvent(mouseEvent);
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
