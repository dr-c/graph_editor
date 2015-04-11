#include "qgraphics_edge.h"
#include "qgraphics_node.h"

#include "weight_text_item.h"

#include <QPen>
#include <QFont>
#include <QKeyEvent>

QGraphicsEdge::QGraphicsEdge(WeightedEdge *edge, QGraphicsItem *parent)
    : QGraphicsPathItem(parent),
      _edge(edge),
      _weightItem(new WeightEdgeTextItem(QString::number(_edge->weight()), this)),
      _simplePen(QPen(QColor(Qt::black))),
      _hoverPen(QPen(QColor(Qt::red)))
{
    _edge->setGraphicsEdge(this);
    setZValue(0.);

    QGraphicsPathItem::setPen(_simplePen);

    _weightItem->hide();

    connect(_weightItem, SIGNAL(textChanged(int)), this, SLOT(setWeight(int)));
    connect(_weightItem, SIGNAL(deleteKeyPressed()), this, SLOT(deleteCompletely()));
}

QGraphicsEdge::~QGraphicsEdge()
{
    if (_edge != nullptr)
        _edge->setGraphicsEdge(nullptr);
}

void QGraphicsEdge::deleteCompletely()
{
    int weight = _edge->weight();
    _edge->remove();
    _edge = nullptr;
    emit removed(weight);
    delete this;
}

WeightedEdge *QGraphicsEdge::edge() const
{
    return _edge;
}

void QGraphicsEdge::setFont(const QFont &font, const QColor &color)
{
    _weightItem->setFont(font);
    _weightItem->setDefaultTextColor(color);
}

QFont QGraphicsEdge::font() const
{
    return _weightItem->font();
}

void QGraphicsEdge::setBrush(const QBrush &brush)
{
    QGraphicsPathItem::setBrush(brush);
    _weightItem->setBrush(brush);
}

QBrush QGraphicsEdge::brush() const
{
    return QGraphicsPathItem::brush();
}

void QGraphicsEdge::setPen(const QPen &pen)
{
    _simplePen = pen;
    QGraphicsPathItem::setPen(_simplePen);
}

QPen QGraphicsEdge::pen() const
{
    return _simplePen;
}

void QGraphicsEdge::setHoverPen(const QPen &pen)
{
    _hoverPen = pen;
}

QPen QGraphicsEdge::hoverPen() const
{
    return _hoverPen;
}

void QGraphicsEdge::setPenTransparency(int minWeight, int maxWeight)
{
    const qreal d_weight = maxWeight - minWeight;
    const qreal alpha = (d_weight == 0) ? 1 : 0.3 + (_edge->weight() - minWeight) / d_weight * 0.7;
    QColor color = _simplePen.color();
    color.setAlphaF(alpha);
    _simplePen.setColor(color);
    setPen(_simplePen);
}

void QGraphicsEdge::join(QGraphicsNode *fromNode, QGraphicsNode *toNode)
{
    _edge->setNodes(fromNode->node(), toNode->node());
    refresh();
    emit created(this);
}

void QGraphicsEdge::refresh()
{
    if (_edge->fromNode()->graphicsNode()->intersects(_edge->toNode()->graphicsNode()))
    {
        if (isVisible())
            hide();
    }
    else
    {
        if (!isVisible())
            show();
        draw(_edge->fromNode()->graphicsNode(), _edge->toNode()->graphicsNode());
    }
}

void QGraphicsEdge::keyPressEvent(QKeyEvent *event)
{
    QGraphicsPathItem::keyPressEvent(event);
    if (event->key() == Qt::Key_Delete)
        deleteCompletely();
}

void QGraphicsEdge::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsPathItem::setPen(_hoverPen);
    QGraphicsPathItem::hoverEnterEvent(event);
}

void QGraphicsEdge::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsPathItem::setPen(_simplePen);
    QGraphicsPathItem::hoverLeaveEvent(event);
}

void QGraphicsEdge::focusInEvent(QFocusEvent *event)
{
    QGraphicsPathItem::focusInEvent(event);
    _weightItem->setFocus();
}

void QGraphicsEdge::showWeight()
{
    _weightItem->show();
}

void QGraphicsEdge::setWeight(int weight)
{
    int old_weight = _edge->weight();
    _edge->setWeight(weight);
    _weightItem->placeInCenter();
    emit changed(old_weight, this);
}
