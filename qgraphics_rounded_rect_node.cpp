#include "qgraphics_rounded_rect_node.h"

QGraphicsRoundedRectNode::QGraphicsRoundedRectNode(WeightedNode *node, qreal roundingCoef, QGraphicsItem *parent)
    : QGraphicsNode(node, parent)
{
    _roundingCoefficient = checkInRange(roundingCoef, 0., 0.3);
    _roundingRadius = _roundingCoefficient * _radius;

    _idItem = new QGraphicsSimpleTextItem(QString::number(_node->id()), this);

    setGeometry(_node->pos());
}

QGraphicsRoundedRectNode::~QGraphicsRoundedRectNode()
{
    delete _idItem;
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

    painter->setPen(_pen);
    painter->setBrush(_brush);

    const qreal diameter = _radius * 2;
    painter->drawRoundedRect(0, 0, diameter, diameter, _roundingRadius, _roundingRadius);
}

void QGraphicsRoundedRectNode::setPen(const QPen &pen)
{
    _pen = pen;
}

void QGraphicsRoundedRectNode::setFont(const QFont &font, const QColor &color)
{
    Q_UNUSED(color);
    _idItem->setFont(font);
}

void QGraphicsRoundedRectNode::setBrush(const QBrush &brush)
{
    _brush = brush;
}

QPen QGraphicsRoundedRectNode::pen() const
{
    return _pen;
}

QFont QGraphicsRoundedRectNode::font() const
{
    return _idItem->font();
}

QBrush QGraphicsRoundedRectNode::brush() const
{
    return _brush;
}

void QGraphicsRoundedRectNode::setGeometry(const QPointF &centerPos)
{
    setPos(centerPos.x() - _radius, centerPos.y() - _radius);

    const QRectF id_item_rect = _idItem->boundingRect();
    _idItem->setPos(_radius - id_item_rect.width() / 2, _radius - id_item_rect.height() / 2);
}

void QGraphicsRoundedRectNode::calcRadius(int weight)
{
    QGraphicsNode::calcRadius(weight);
    _roundingRadius = _radius * _roundingCoefficient;
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
