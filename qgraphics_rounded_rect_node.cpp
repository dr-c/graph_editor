#include "qgraphics_rounded_rect_node.h"

#include "weight_text_item.h"

QGraphicsRoundedRectNode::QGraphicsRoundedRectNode(WeightedNode *node, qreal lineShiftCoef, qreal roundingCoef, QGraphicsItem *parent)
    : QGraphicsNode(node, parent),
      _lineItem(new QGraphicsLineItem(this)),
      _lineShiftCoefficient(checkInRange(lineShiftCoef, 0.5, 1.)),
      _roundingCoefficient(checkInRange(roundingCoef, 0., 0.3)),
      _roundingRadius(_roundingCoefficient * _radius),
      _pen(pen())
{
    setGeometry(_node->pos());
}

QGraphicsRoundedRectNode::~QGraphicsRoundedRectNode()
{
    delete _lineItem;
}

int QGraphicsRoundedRectNode::type() const
{
    return Type;
}

QRectF QGraphicsRoundedRectNode::boundingRect() const
{
    const qreal half_pen_width = - _pen.widthF() / 2;
    const qreal full_diameter = _radius * 2 + _pen.widthF();
    return QRectF(half_pen_width, half_pen_width, full_diameter, full_diameter);
}

void QGraphicsRoundedRectNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (this->isSelected())
    {
        QPen pen;
        pen.setStyle(Qt::DashLine);
        painter->setPen(pen);
        painter->drawRect(boundingRect());
    }

    painter->setPen(_pen);
    painter->setBrush(_brush);

    const qreal diameter = _radius * 2;
    painter->drawRoundedRect(0, 0, diameter, diameter, _roundingRadius, _roundingRadius);
}

void QGraphicsRoundedRectNode::setFont(const QFont &font, const QColor &color)
{
    _weightItem->setFont(font);
    _weightItem->setDefaultTextColor(color);
    _idItem->setFont(font);
    setGeometry(_node->pos());
}

void QGraphicsRoundedRectNode::setBrush(const QBrush &brush)
{
    _brush = brush;
    update(boundingRect());
}

QFont QGraphicsRoundedRectNode::font() const
{
    return _idItem->font();
}

QBrush QGraphicsRoundedRectNode::brush() const
{
    return _brush;
}

bool QGraphicsRoundedRectNode::intersects(QGraphicsNode *gnode) const
{
    QPointF difference = _node->pos() - gnode->node()->pos();
    qreal required_distance = _radius + gnode->radius();
    return abs(difference.x()) <= required_distance && abs(difference.y()) <= required_distance;
}

QPainterPath QGraphicsRoundedRectNode::shape() const
{
    QPainterPath path;
    const qreal diameter = _radius * 2;
    path.addRoundedRect(0, 0, diameter, diameter, _roundingRadius, _roundingRadius);
    return path;
}

void QGraphicsRoundedRectNode::setCoefficients(qreal lineShiftCoef, qreal roundingCoef)
{
    _lineShiftCoefficient = checkInRange(lineShiftCoef, 0.5, 1.);
    _roundingCoefficient = checkInRange(roundingCoef, 0., 0.3);
    _roundingRadius = _roundingCoefficient * _radius;
    setGeometry(_node->pos());
}

void QGraphicsRoundedRectNode::setGeometry(const QPointF &centerPos)
{
    setPos(centerPos.x() - _radius, centerPos.y() - _radius);

    const QRectF weight_item_rect = _weightItem->boundingRect();
    const qreal shift_to_weight_item_center = _radius * (2 - _lineShiftCoefficient / 2) - _roundingRadius / 2;
    _weightItem->setPos(shift_to_weight_item_center - weight_item_rect.width() / 2,
                        shift_to_weight_item_center - weight_item_rect.height() / 2);

    const qreal line_item_rect_shift = (1 - _lineShiftCoefficient) * _radius * 2;
    _lineItem->setPos(line_item_rect_shift, line_item_rect_shift);
    const qreal line_projection_length = _lineShiftCoefficient * _radius * 2 - _pen.widthF() / 2;
    _lineItem->setLine(0, line_projection_length, line_projection_length, 0);

    const QRectF id_item_rect = _idItem->boundingRect();
    const qreal shift_to_id_item_center = (_radius * (2 - _lineShiftCoefficient) + _roundingRadius) / 2;
    _idItem->setPos(shift_to_id_item_center - id_item_rect.width() / 2,
                    shift_to_id_item_center - id_item_rect.height() / 2);
}

void QGraphicsRoundedRectNode::setActivePen(const QPen &pen)
{
    _pen = pen;
    _lineItem->setPen(_pen);
    update(boundingRect());
}

void QGraphicsRoundedRectNode::calcRadius(int weight)
{
    QGraphicsNode::calcRadius(weight);
    _roundingRadius = _radius * _roundingCoefficient;
}

QPointF QGraphicsRoundedRectNode::calcIntermediatePoint(const QPointF &toPoint)
{
    QPointF fromPoint = _node->pos();

    qreal DX = fromPoint.x() - toPoint.x();
    qreal DY = fromPoint.y() - toPoint.y();

    qreal dx, dy;

    if (abs(DY) > abs(DX))
    {
       dy = (DY < 0) ? _radius : -_radius;
       dx = (DX == 0) ? 0 : dy * DX / DY;
    }
    else
    {
       dx = (DX < 0) ? _radius : -_radius;
       dy = (DY == 0) ? 0 : dx * DY / DX;
    }
    return QPointF(fromPoint.x() + dx, fromPoint.y() + dy);
}

qreal QGraphicsRoundedRectNode::checkInRange(qreal var, qreal from, qreal to)
{
    assert(from <= to);
    if (var < from)
        return from;
    if (var > to)
        return to;
    return var;
}
