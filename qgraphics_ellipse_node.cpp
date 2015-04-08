#include "qgraphics_ellipse_node.h"

QGraphicsEllipseNode::QGraphicsEllipseNode(WeightedNode *node, QGraphicsItem *parent)
    : QGraphicsNode(node, parent)
{
    _ellipseItem = new QGraphicsEllipseItem(this);
    _idItem = new QGraphicsSimpleTextItem(QString::number(_node->id()), this);
    const QPointF &center = node->pos();
    _ellipseItem->setRect(center.x() - 10, center.y() - 10, center.x() + 10, center.y() + 10);
    _idItem->setPos(center.x() - 5, center.y() - 5);
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
