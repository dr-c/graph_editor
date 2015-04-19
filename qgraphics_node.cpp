#include "qgraphics_node.h"
#include "qgraphics_edge.h"

#include "weight_text_item.h"
#include "graph_scene.h"

#include <QKeyEvent>

QGraphicsNode::QGraphicsNode(BasicGraphScene *scene, WeightedNode *node, QGraphicsItem *parent)
    : QGraphicsObject(parent),
      _scene(scene),
      _node(node),
      _weightItem(new WeightTextItem(_node->weight(), this)),
      _idItem(new QGraphicsSimpleTextItem(QString::number(_node->id()), this)),
      _simplePen(QPen(QColor(Qt::black))),
      _hoverPen(QPen(QColor(Qt::red))),
      _beforeWeightChanging(_node->weight())
{
    _node->setGraphicsNode(this);
    calcRadius(_node->weight());
    _weightItem->setZValue(1);
    _idItem->setZValue(1);

    connect(_weightItem, SIGNAL(textChanged(int)), this, SLOT(setWeight(int)));
    connect(_weightItem, SIGNAL(finishTextChanging(int)), this, SLOT(weightFixed(int)));
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
            delete pair.second->graphicsEdge();
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

const QPen &QGraphicsNode::pen() const
{
    return _simplePen;
}

void QGraphicsNode::setHoverPen(const QPen &pen)
{
    _hoverPen = pen;
}

const QPen &QGraphicsNode::hoverPen() const
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

void QGraphicsNode::changePositionOutside(const QPointF &topLeft)
{
    setPos(topLeft);
    _node->setPos(topLeft + QPointF(_radius, _radius));
    updateRelatedEdges();
}

void QGraphicsNode::changeWeightOutside(int weight)
{
    _weightItem->setPlainText(QString::number(weight));
    setWeight(weight);
}

void QGraphicsNode::setWeight(int weight)
{
    const QPointF center = _node->pos();
    calcRadius(weight);
    setGeometry(center);
    updateRelatedEdges();
    _node->setWeight(weight);
}

void QGraphicsNode::weightFixed(int weight)
{
    if (_beforeWeightChanging != weight)
        _scene->history()->writeNodeWeightChanging(this, _beforeWeightChanging, weight);
}

void QGraphicsNode::calcRadius(int weight)
{
    if (weight < 0)
        weight = 0;
    _radius = sqrt(weight + RADIUS_SHIFT) * RADIUS_SCALE;
    setZValue(1000./_radius);
}

void QGraphicsNode::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    _beforeMousePressPos = pos();
    QGraphicsObject::mousePressEvent(mouseEvent);
}

void QGraphicsNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPointF after_mouse_press_pos = pos();
    if (_beforeMousePressPos != after_mouse_press_pos)
    {
        const QList<QGraphicsItem*> &listItems = _scene->selectedItems();
        assert(!listItems.empty());
        if (listItems.count() == 1)
            _scene->history()->writeNodeMoving(this, _beforeMousePressPos, after_mouse_press_pos);
        else
            _scene->history()->writeGroupNodesMoving(listItems, _beforeMousePressPos, after_mouse_press_pos);
    }
    QGraphicsObject::mouseReleaseEvent(mouseEvent);
}

void QGraphicsNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);
    _beforeWeightChanging = _node->weight();
    _weightItem->setTextInteractionFlags(Qt::TextEditorInteraction);
    _weightItem->setFocus();
}

void QGraphicsNode::keyPressEvent(QKeyEvent *event)
{
    QGraphicsObject::keyPressEvent(event);
    if (event->key() == Qt::Key_Delete)
    {
        _scene->history()->writeNodeDeletion(this);
        deleteCompletely();
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
