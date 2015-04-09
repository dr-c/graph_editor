#include "qgraphics_simple_line_edge.h"

#include "qgraphics_node.h"

QGraphicsSimpleLineEdge::QGraphicsSimpleLineEdge(WeightedEdge *edge, QGraphicsItem *parent)
    : QGraphicsEdge(edge, parent)
{

}

QGraphicsSimpleLineEdge::~QGraphicsSimpleLineEdge()
{

}

int QGraphicsSimpleLineEdge::type() const
{
    return Type;
}

void QGraphicsSimpleLineEdge::draw(QGraphicsNode *fromNode, QGraphicsNode *toNode)
{
    QPainterPath path(fromNode->calcIntermediatePoint(toNode->node()->pos()));
    path.lineTo(toNode->calcIntermediatePoint(fromNode->node()->pos()));
    setPath(path);
}

void QGraphicsSimpleLineEdge::draw(QGraphicsNode *fromNode, const QPointF &toPoint)
{
    QPainterPath path(fromNode->calcIntermediatePoint(toPoint));
    path.lineTo(toPoint);
    setPath(path);
}
