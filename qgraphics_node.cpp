#include "qgraphics_node.h"
#include "qgraphics_edge.h"

#include "weight_text_item.h"

#include <QKeyEvent>

QGraphicsNode::QGraphicsNode(WeightedNode *node, QGraphicsItem *parent)
    : QGraphicsObject(parent),
      _node(node),
      _weightItem(new WeightTextItem(_node->weight(), this)),
      _idItem(new QGraphicsSimpleTextItem(QString::number(_node->id()), this)),
      _simplePen(QPen(QColor(Qt::black))),
      _hoverPen(QPen(QColor(Qt::red)))
{
    _node->setGraphicsNode(this);
    calcRadius(_node->weight());
    _weightItem->setZValue(1);
    _idItem->setZValue(1);

    connect(_weightItem, SIGNAL(textChanged(int)), this, SLOT(setWeight(int)));
}

QGraphicsNode::~QGraphicsNode()
{
    delete _idItem;
    delete _weightItem;
    if (_node != nullptr)
        _node->setGraphicsNode(nullptr);
}

void QGraphicsNode::deleteCompletely()
{
    _node->for_each([](std::pair<WeightedNode* const, WeightedEdge*>& pair){
        if (pair.second->graphicsEdge() != nullptr)
            pair.second->graphicsEdge()->deleteCompletely();
    });
    _node->remove();
    _node = nullptr;
    delete this;
}

void QGraphicsNode::setPen(const QPen &pen)
{
    _simplePen = pen;
    setActivePen(_simplePen);
}

QPen QGraphicsNode::pen() const
{
    return _simplePen;
}

void QGraphicsNode::setHoverPen(const QPen &pen)
{
    _hoverPen = pen;
}

QPen QGraphicsNode::hoverPen() const
{
    return _hoverPen;
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
    const QPointF center = _node->pos();
    calcRadius(weight);
    setGeometry(center);
    updateRelatedEdges();
    _node->setWeight(weight);
}

void QGraphicsNode::calcRadius(int weight)
{
    if (weight < 0)
        weight = 0;
    _radius = sqrt(weight + RADIUS_SHIFT) * RADIUS_SCALE;
    setZValue(1000./_radius);
}

void QGraphicsNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);
    _weightItem->setTextInteractionFlags(Qt::TextEditorInteraction);
    _weightItem->setFocus();
}

void QGraphicsNode::keyPressEvent(QKeyEvent *event)
{
    QGraphicsObject::keyPressEvent(event);
    if (event->key() == Qt::Key_Delete)
        deleteCompletely();
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

void QGraphicsNode::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setActivePen(_hoverPen);
    QGraphicsObject::hoverEnterEvent(event);
}

void QGraphicsNode::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setActivePen(_simplePen);
    QGraphicsObject::hoverLeaveEvent(event);
}

void QGraphicsNode::updateRelatedEdges()
{
    _node->for_each([](std::pair<WeightedNode* const, WeightedEdge*>& pair){ pair.second->graphicsEdge()->refresh(); });
}
