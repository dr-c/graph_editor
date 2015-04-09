#include "qgraphics_node.h"

QGraphicsNode::QGraphicsNode(WeightedNode *node, QGraphicsItem *parent)
    : QGraphicsObject(parent), _node(node)
{
    _node->setGraphicsNode(this);
    calcRadius(_node->weight());
}

QGraphicsNode::~QGraphicsNode()
{

}


qreal QGraphicsNode::radius() const
{
    return _radius;
}

WeightedNode *QGraphicsNode::node() const
{
    return _node;
}

void QGraphicsNode::setWeight(int weight)
{
    _node->setWeight(weight);
    calcRadius(weight);
    setGeometry(_node->pos());
}

void QGraphicsNode::calcRadius(int weight)
{
    if (weight < 0)
        weight = 0;
    _radius = sqrt(weight + RADIUS_SHIFT) * RADIUS_SCALE;
}
