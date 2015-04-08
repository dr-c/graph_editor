#include "qgraphics_node.h"

QGraphicsNode::QGraphicsNode(WeightedNode *node, QGraphicsItem *parent)
    : QGraphicsObject(parent), _node(node)
{

}

QGraphicsNode::~QGraphicsNode()
{

}
