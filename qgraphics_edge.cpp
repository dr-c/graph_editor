#include "qgraphics_edge.h"
#include "qgraphics_node.h"

QGraphicsEdge::QGraphicsEdge(WeightedEdge *edge, QGraphicsItem *parent)
    : QGraphicsPathItem(parent), _edge(edge)
{
    _edge->setGraphicsEdge(this);
}

QGraphicsEdge::~QGraphicsEdge()
{

}

WeightedEdge *QGraphicsEdge::edge() const
{
    return _edge;
}

void QGraphicsEdge::connect(QGraphicsNode *fromNode, QGraphicsNode *toNode)
{
    _edge->setNodes(fromNode->node(), toNode->node());
    draw(fromNode, toNode);
}

void QGraphicsEdge::update()
{
    draw(_edge->fromNode()->graphicsNode(), _edge->toNode()->graphicsNode());
}
