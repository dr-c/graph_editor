#include "qgraphics_cubic_arrow_edge.h"

#include "qgraphics_node.h"
#include "weight_text_item.h"

QGraphicsCubicArrowEdge::QGraphicsCubicArrowEdge(BasicGraphScene *scene, WeightedEdge *edge, QGraphicsItem *parent)
    : QGraphicsEdge(scene, edge, parent),
      _curve(new CurvePathItem(this))
{
    _curve->setZValue(0);
    _weightItem->setZValue(1);
}

QGraphicsCubicArrowEdge::~QGraphicsCubicArrowEdge()
{
    delete _curve;
}

void QGraphicsCubicArrowEdge::draw(QGraphicsNode *fromGNode, QGraphicsNode *toGNode)
{
    const QPointF from_interm_point = fromGNode->calcIntermediatePoint(toGNode->node()->pos());
    const QPointF to_interm_point = toGNode->calcIntermediatePoint(fromGNode->node()->pos());

    QPainterPath path_curve(from_interm_point);
    QPointF start_arrow_point = calcIntermediatePoint(to_interm_point, from_interm_point, 20);
    path_curve.cubicTo(calcCubicPoint(from_interm_point, start_arrow_point, 0.3, 30), calcCubicPoint(start_arrow_point, from_interm_point, 0.3, 0), start_arrow_point);
    _curve->setPath(path_curve);

    QPainterPath path_triangle(to_interm_point);
    path_triangle.addPolygon(calcTrianglePolygon(from_interm_point, to_interm_point, 20, 6));
    setPath(path_triangle);

    _weightItem->setCenterPoint(calcCubicPoint(from_interm_point, to_interm_point, 0.5, 10));
    _weightItem->placeInCenter();
}

void QGraphicsCubicArrowEdge::draw(QGraphicsNode *fromGNode, const QPointF &toPoint)
{
    const QPointF from_interm_point = fromGNode->calcIntermediatePoint(toPoint);

    QPainterPath path_curve(from_interm_point);
    QPointF start_arrow_point = calcIntermediatePoint(toPoint, from_interm_point, 20);
    path_curve.cubicTo(calcCubicPoint(from_interm_point, start_arrow_point, 0.3, 30), calcCubicPoint(start_arrow_point, from_interm_point, 0.3, 0), start_arrow_point);
    _curve->setPath(path_curve);

    QPainterPath path_triangle(toPoint);
    path_triangle.addPolygon(calcTrianglePolygon(from_interm_point, toPoint, 20, 6));
    setPath(path_triangle);
}

void QGraphicsCubicArrowEdge::setActivePen(const QPen &pen)
{
    QGraphicsPathItem::setPen(pen);
    _curve->setPen(pen);
}

int QGraphicsCubicArrowEdge::type() const
{
    return Type;
}

QPointF QGraphicsCubicArrowEdge::calcCubicPoint(const QPointF &pointFrom, const QPointF &pointTo, const qreal len_persentage, const qreal height)
{
    qreal dx = (pointFrom.x() - pointTo.x());
    qreal dy = (pointFrom.y() - pointTo.y());
    qreal len = sqrt(dx * dx + dy * dy);
    QPointF pointDividing = calcIntermediatePoint(pointFrom, pointTo, len_persentage * len);
    if (dy == 0)
        return QPoint(pointDividing.x(), pointDividing.y() + (dx > 0 ? height : -height));

    qreal ratio = height / len;
    qreal shift_x = - dy * ratio;
    qreal shift_y = dx * ratio;
    return QPointF(pointDividing.x() + shift_x, pointDividing.y() + shift_y);
}

QPolygonF QGraphicsCubicArrowEdge::calcTrianglePolygon(const QPointF &pointFrom, const QPointF &pointTo, const qreal height, const qreal width)
{
    QPolygonF polygon;
    polygon << pointTo;
    QPointF pointDividing = calcIntermediatePoint(pointTo, pointFrom, height);
    qreal dx = (pointFrom.x() - pointTo.x());
    qreal dy = (pointFrom.y() - pointTo.y());
    if (dy == 0)
        return polygon << QPointF(pointDividing.x(), pointDividing.y()- width) << QPointF(pointDividing.x(), pointDividing.y() + width) << pointTo;

    qreal ratio = width / sqrt(dx * dx + dy * dy);
    qreal shift_x = - dy * ratio;
    qreal shift_y = dx * ratio;

    return polygon << QPointF(pointDividing.x() + shift_x, pointDividing.y() + shift_y) << QPointF(pointDividing.x() - shift_x, pointDividing.y() - shift_y) << pointTo;
}
