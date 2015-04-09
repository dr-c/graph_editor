#include "qgraphics_node.h"
#include "qgraphics_edge.h"

#include <QKeyEvent>

QGraphicsNode::QGraphicsNode(WeightedNode *node, QGraphicsItem *parent)
    : QGraphicsObject(parent), _node(node)
{
    _node->setGraphicsNode(this);
    calcRadius(_node->weight());
}

QGraphicsNode::~QGraphicsNode()
{
    _node->for_each([](std::pair<WeightedNode* const, WeightedEdge*>& pair){ delete pair.second->graphicsEdge(); });
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
    updateRelatedEdges();
}

void QGraphicsNode::calcRadius(int weight)
{
    if (weight < 0)
        weight = 0;
    _radius = sqrt(weight + RADIUS_SHIFT) * RADIUS_SCALE;
}

void QGraphicsNode::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete)
    {
        delete this;
        _node->remove();
    }
}

QVariant QGraphicsNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionHasChanged)
    {
        _node->setPos(value.toPointF() + QPointF(_radius, _radius));
        updateRelatedEdges();
    }

    return QGraphicsObject::itemChange(change, value);
}

void QGraphicsNode::updateRelatedEdges()
{
    _node->for_each([](std::pair<WeightedNode* const, WeightedEdge*>& pair){ pair.second->graphicsEdge()->update(); });
}
