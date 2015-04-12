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

void QGraphicsEdge::setActivePen(const QPen &pen)
{
    QGraphicsPathItem::setPen(pen);
}

void QGraphicsEdge::setPen(const QPen &pen)
{
    _simplePen = pen;
    setActivePen(_simplePen);
}

const QPen &QGraphicsEdge::pen() const
{
    return _simplePen;
}

void QGraphicsEdge::setHoverPen(const QPen &pen)
{
    _hoverPen = pen;
}

const QPen &QGraphicsEdge::hoverPen() const
{
    return _hoverPen;
}

void QGraphicsEdge::setPenTransparency(int minWeight, int maxWeight)
{
    const qreal weight_diff = maxWeight - minWeight;
    const qreal alpha = (weight_diff == 0) ? 1 : 0.3 + (_edge->weight() - minWeight) / weight_diff * 0.7;
    QColor color = _simplePen.color();
    color.setAlphaF(alpha);
    _simplePen.setColor(color);
    setPen(_simplePen);
}

void QGraphicsEdge::join(QGraphicsNode *fromGNode, QGraphicsNode *toGNode)
{
    _edge->setNodes(fromGNode->node(), toGNode->node());
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

void QGraphicsEdge::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsPathItem::hoverEnterEvent(event);
    setActivePen(_hoverPen);
}

void QGraphicsEdge::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsPathItem::hoverLeaveEvent(event);
    setActivePen(_simplePen);
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

QPointF QGraphicsEdge::calcIntermediatePoint(const QPointF &pointFrom, const QPointF &pointTo, qreal radius)
{
    // y = kx + b, k = dy / dx
    qreal dx = pointFrom.x() - pointTo.x();
    qreal dy = pointFrom.y() - pointTo.y();
    if (dx == 0)
        return QPointF(pointFrom.x(), pointFrom.y() + (dy < 0 ? radius : -radius));

    qreal k = dy / dx;
    qreal deltaX = radius / sqrt(k * k + 1);
    if (dx > 0)
        deltaX = -deltaX;
    return QPointF(pointFrom.x() + deltaX, pointFrom.y() + k * deltaX);
}
