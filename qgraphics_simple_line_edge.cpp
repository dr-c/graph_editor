#include "qgraphics_simple_line_edge.h"

#include "qgraphics_node.h"
#include "weight_text_item.h"

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
    const QPointF from_interm_point = fromNode->calcIntermediatePoint(toNode->node()->pos());
    const QPointF to_interm_point = toNode->calcIntermediatePoint(fromNode->node()->pos());
    QPainterPath path(from_interm_point);
    path.lineTo(to_interm_point);
    setPath(path);
    _weightItem->calcCenterPoint(QRectF(from_interm_point, to_interm_point));
    _weightItem->placeInCenter();
}

void QGraphicsSimpleLineEdge::draw(QGraphicsNode *fromNode, const QPointF &toPoint)
{
    QPainterPath path(fromNode->calcIntermediatePoint(toPoint));
    path.lineTo(toPoint);
    setPath(path);
}
