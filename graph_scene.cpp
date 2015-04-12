#include "graph_scene.h"

#include "graph_scene_mode.h"

#include <QGraphicsSceneMouseEvent>

BasicGraphScene::BasicGraphScene(WeightedGraph *graph, GraphSceneMode *mode, QObject *parent)
    : QGraphicsScene(parent),
      _graph(graph),
      _mode(mode),
      _nodePen(QPen(QColor(Qt::blue), 2)),
      _nodeHoverPen(QPen(QColor(Qt::red), 2)),
      _edgePen(QPen(QColor(Qt::black), 2)),
      _edgeHoverPen(QPen(QColor(Qt::red), 2)),
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
    connect(graphics_node, SIGNAL(destroyed()), this, SLOT(calcEdgesWeightRange()));
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
    graphics_edge->setPen(_edgePen);
    graphics_edge->setFont(_itemFont);
    graphics_edge->setBrush(_edgeBrush);
    graphics_edge->setHoverPen(_edgeHoverPen);
    graphics_edge->setAcceptHoverEvents(false);
    graphics_edge->setFlags(QGraphicsItem::ItemIsFocusable);
    connect(graphics_edge, SIGNAL(created(QGraphicsEdge*)), this, SLOT(calcEdgesTransparencyOnCreate(QGraphicsEdge*)));
    connect(graphics_edge, SIGNAL(changed(int,QGraphicsEdge *)), this, SLOT(calcEdgesTransparencyOnChange(int,QGraphicsEdge *)));
    connect(graphics_edge, SIGNAL(removed(int)), this, SLOT(calcEdgesTransparencyOnDelete(int)));
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

void BasicGraphScene::calcEdgesTransparencyOnCreate(QGraphicsEdge *gEdge)
{
    if (gEdge->edge()->weight() <= _maxEdgeWeight && gEdge->edge()->weight() >= _minEdgeWeight)
        gEdge->setPenTransparency(_minEdgeWeight, _maxEdgeWeight);
    else
        calcEdgesWeightRange();
}

void BasicGraphScene::calcEdgesTransparencyOnChange(int fromWeight, QGraphicsEdge *gEdge)
{
    if (fromWeight != _minEdgeWeight && fromWeight != _maxEdgeWeight &&
            gEdge->edge()->weight() <= _maxEdgeWeight && gEdge->edge()->weight() >= _minEdgeWeight)
        return;

    if (!calcEdgesWeightRange())
        gEdge->setPenTransparency(_minEdgeWeight, _maxEdgeWeight);
}

void BasicGraphScene::calcEdgesTransparencyOnDelete(int weight)
{
    if (_graph->edges().empty())
    {
        _minEdgeWeight = _maxEdgeWeight = 0;
        return;
    }

    if (_minEdgeWeight == _maxEdgeWeight || (weight != _minEdgeWeight && weight != _maxEdgeWeight))
        return;

    calcEdgesWeightRange();
}

bool BasicGraphScene::calcEdgesWeightRange()
{
    int min_weight = std::numeric_limits<int>::max();
    int max_weight = std::numeric_limits<int>::min();

    for (auto edge : _graph->edges())
    {
        if (edge->weight() < min_weight)
            min_weight = edge->weight();
        if (edge->weight() > max_weight)
            max_weight = edge->weight();
    }

    if (min_weight == _minEdgeWeight && max_weight == _maxEdgeWeight)
        return false;

    _minEdgeWeight = min_weight;
    _maxEdgeWeight = max_weight;

    for (auto edge : _graph->edges())
        edge->graphicsEdge()->setPenTransparency(_minEdgeWeight, _maxEdgeWeight);
    return true;
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
