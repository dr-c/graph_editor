#include "graph_scene.h"

#include "graph_scene_mode.h"

#include <QGraphicsSceneMouseEvent>

GraphScene::GraphScene(std::shared_ptr<WeightedGraph> &&graph, std::shared_ptr<GraphConfiguration> &&config, GraphSceneMode *mode, QObject *parent)
    : QGraphicsScene(parent),
      _history(new History(this)),
      _graph(graph),
      _config(config),
      _mode(mode),
      _minEdgeWeight(0),
      _maxEdgeWeight(0)
{
    _mode->setScene(this);
}

GraphScene::~GraphScene()
{
    delete _mode;
    delete _history;
}

QGraphicsNode *GraphScene::addNode(const QPointF &centerPos, int weight, int id)
{
    WeightedNode *node = _graph->createNode(id);
    node->setPos(centerPos);
    node->setWeight(weight);
    return addNode(node);
}

QGraphicsNode *GraphScene::addNode(WeightedNode *node)
{
    QGraphicsNode *graphics_node = _config->_nodeCreator->create(this, node);
    setGNodeDesign(graphics_node);
    graphics_node->setAcceptHoverEvents(true);
    _mode->setGNodeFlags(graphics_node);
    connect(graphics_node, SIGNAL(destroyed()), this, SLOT(calcEdgesWeightRange()));
    addItem(graphics_node);
    return graphics_node;
}

QGraphicsEdge *GraphScene::addEdge(int weight)
{
    WeightedEdge *edge = _graph->createEdge();
    edge->setWeight(weight);
    return addEdge(edge);
}

QGraphicsEdge *GraphScene::addEdge(WeightedEdge *edge)
{
    QGraphicsEdge *graphics_edge = _config->_edgeCreator->create(this, edge);
    setGEdgeDesign(graphics_edge);
    graphics_edge->setAcceptHoverEvents(false);
    graphics_edge->setFlags(QGraphicsItem::ItemIsFocusable);
    connect(graphics_edge, SIGNAL(created(QGraphicsEdge*)), this, SLOT(calcEdgesTransparencyOnCreate(QGraphicsEdge*)));
    connect(graphics_edge, SIGNAL(changed(int,QGraphicsEdge *)), this, SLOT(calcEdgesTransparencyOnChange(int,QGraphicsEdge *)));
    connect(graphics_edge, SIGNAL(removed(int)), this, SLOT(calcEdgesTransparencyOnDelete(int)));
    addItem(graphics_edge);
    return graphics_edge;
}

History *GraphScene::history() const
{
    return _history;
}

const std::shared_ptr<WeightedGraph> &GraphScene::graph() const
{
    return _graph;
}

const GraphSceneMode *GraphScene::mode() const
{
    return _mode;
}

void GraphScene::setMode(GraphSceneMode *mode)
{
    delete _mode;
    _mode = mode;
    for (auto pair : _graph->nodes())
        _mode->setGNodeFlags(pair.second->graphicsNode());
}

const std::shared_ptr<GraphConfiguration> &GraphScene::config()
{
    return _config;
}

void GraphScene::setConfig(std::shared_ptr<GraphConfiguration> &&config)
{
    _config.swap(config);
    if (config->_nodeCreator->type() != _config->_nodeCreator->type())
    {
        for (auto pair : _graph->nodes())
        {
            delete pair.second->graphicsNode();
            addNode(pair.second);
        }
    }
    else if (config->_nodePen!= _config->_nodePen || config->_nodeHoverPen!= _config->_nodeHoverPen ||
             config->_nodeBrush != _config->_nodeBrush || config->_nodeFont != _config->_nodeFont)
    {
        for (auto pair : _graph->nodes())
            setGNodeDesign(pair.second->graphicsNode());
    }

    if (config->_edgeCreator->type() != _config->_edgeCreator->type())
    {
        for (auto edge : _graph->edges())
        {
            delete edge->graphicsEdge();
            QGraphicsEdge *graphics_edge = addEdge(edge);
            graphics_edge->refresh();
            graphics_edge->showWeight();
        }
        _minEdgeWeight = _maxEdgeWeight = 0;
        calcEdgesWeightRange();
    }
    else if (config->_edgePen!= _config->_edgePen || config->_edgeHoverPen!= _config->_edgeHoverPen ||
             config->_edgeBrush != _config->_edgeBrush || config->_edgeFont != _config->_edgeFont)
    {
        for (auto edge : _graph->edges())
            setGEdgeDesign(edge->graphicsEdge());
        _minEdgeWeight = _maxEdgeWeight = 0;
        calcEdgesWeightRange();
    }
}

void GraphScene::calcEdgesTransparencyOnCreate(QGraphicsEdge *gEdge)
{
    if (gEdge->edge()->weight() <= _maxEdgeWeight && gEdge->edge()->weight() >= _minEdgeWeight)
        gEdge->setPenTransparency(_minEdgeWeight, _maxEdgeWeight);
    else
        calcEdgesWeightRange();
}

void GraphScene::calcEdgesTransparencyOnChange(int fromWeight, QGraphicsEdge *gEdge)
{
    if (fromWeight != _minEdgeWeight && fromWeight != _maxEdgeWeight &&
            gEdge->edge()->weight() <= _maxEdgeWeight && gEdge->edge()->weight() >= _minEdgeWeight)
        return;

    if (!calcEdgesWeightRange())
        gEdge->setPenTransparency(_minEdgeWeight, _maxEdgeWeight);
}

void GraphScene::calcEdgesTransparencyOnDelete(int weight)
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

bool GraphScene::calcEdgesWeightRange()
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

void GraphScene::setGNodeDesign(QGraphicsNode *gNode)
{
    gNode->setPen(_config->_nodePen);
    gNode->setFont(_config->_nodeFont);
    gNode->setBrush(_config->_nodeBrush);
    gNode->setHoverPen(_config->_nodeHoverPen);
}

void GraphScene::setGEdgeDesign(QGraphicsEdge *gEdge)
{
    gEdge->setPen(_config->_edgePen);
    gEdge->setFont(_config->_edgeFont);
    gEdge->setBrush(_config->_edgeBrush);
    gEdge->setHoverPen(_config->_edgeHoverPen);
}

void GraphScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    _mode->mouseDoubleClickEvent(mouseEvent);
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    _mode->mouseMoveEvent(mouseEvent);
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    _mode->mousePressEvent(mouseEvent);
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    _mode->mouseReleaseEvent(mouseEvent);
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void GraphScene::keyPressEvent(QKeyEvent *keyEvent)
{
    _mode->keyPressEvent(keyEvent);
    QGraphicsScene::keyPressEvent(keyEvent);
}

void GraphScene::keyReleaseEvent(QKeyEvent *keyEvent)
{
    _mode->keyReleaseEvent(keyEvent);
    QGraphicsScene::keyReleaseEvent(keyEvent);
}
