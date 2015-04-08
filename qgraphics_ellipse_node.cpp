#include "qgraphics_ellipse_node.h"

QGraphicsEllipseNode::QGraphicsEllipseNode(WeightedNode *node, QGraphicsItem *parent)
    : QGraphicsNode(node, parent)
{
    _ellipseItem = new QGraphicsEllipseItem(this);
    _idItem = new QGraphicsSimpleTextItem(QString::number(_node->id()), this);
    const QPointF &center = node->pos();
    _ellipseItem->setRect(center.x() - 10, center.y() - 10, 20, 20);
    _idItem->setPos(center.x() - 5, center.y() - 10);
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
