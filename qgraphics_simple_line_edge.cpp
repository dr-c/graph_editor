#include "qgraphics_simple_line_edge.h"

#include "qgraphics_node.h"
#include "weight_text_item.h"

QGraphicsSimpleLineEdge::QGraphicsSimpleLineEdge(BasicGraphScene *scene, WeightedEdge *edge, QGraphicsItem *parent)
    : QGraphicsEdge(scene, edge, parent)
{

}

int QGraphicsSimpleLineEdge::type() const
{
    return Type;
}

void QGraphicsSimpleLineEdge::draw(QGraphicsNode *fromGNode, QGraphicsNode *toGNode)
{
    const QPointF from_interm_point = fromGNode->calcIntermediatePoint(toGNode->node()->pos());
    const QPointF to_interm_point = toGNode->calcIntermediatePoint(fromGNode->node()->pos());
    QPainterPath path(from_interm_point);
    path.lineTo(to_interm_point);
    setPath(path);
    _weightItem->calcCenterPoint(QRectF(from_interm_point, to_interm_point));
    _weightItem->placeInCenter();
}

void QGraphicsSimpleLineEdge::draw(QGraphicsNode *fromGNode, const QPointF &toPoint)
{
    QPainterPath path(fromGNode->calcIntermediatePoint(toPoint));
    path.lineTo(toPoint);
    setPath(path);
}
