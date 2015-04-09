#include "qgraphics_ellipse_node.h"

#include <QPainter>

QGraphicsEllipseNode::QGraphicsEllipseNode(WeightedNode *node, QGraphicsItem *parent)
    : QGraphicsNode(node, parent)
{
    _ellipseItem = new QGraphicsEllipseItem(this);
    _idItem = new QGraphicsSimpleTextItem(QString::number(_node->id()), this);

    setGeometry(_node->pos());
}

QGraphicsEllipseNode::~QGraphicsEllipseNode()
{
    delete _ellipseItem;
    delete _idItem;
}

int QGraphicsEllipseNode::type() const
{
    return Type;
}

QRectF QGraphicsEllipseNode::boundingRect() const
{
    return _ellipseItem->boundingRect();
}

void QGraphicsEllipseNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void QGraphicsEllipseNode::setPen(const QPen &pen)
{
    _ellipseItem->setPen(pen);
}

void QGraphicsEllipseNode::setFont(const QFont &font, const QColor &color)
{
    Q_UNUSED(color);
    _idItem->setFont(font);
}

void QGraphicsEllipseNode::setBrush(const QBrush &brush)
{
    _ellipseItem->setBrush(brush);
}

QPen QGraphicsEllipseNode::pen() const
{
    return _ellipseItem->pen();
}

QFont QGraphicsEllipseNode::font() const
{
    return _idItem->font();
}

QBrush QGraphicsEllipseNode::brush() const
{
    return _ellipseItem->brush();
}

void QGraphicsEllipseNode::setGeometry(const QPointF &centerPos)
{
    setPos(centerPos.x() - _radius, centerPos.y() - _radius);

    const qreal diameter = _radius * 2;
    _ellipseItem->setRect(0, 0, diameter, diameter);

    const QRectF id_item_rect = _idItem->boundingRect();
    _idItem->setPos(_radius - id_item_rect.width() / 2, _radius - id_item_rect.height() / 2);
}
