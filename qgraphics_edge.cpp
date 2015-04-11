#include "qgraphics_edge.h"
#include "qgraphics_node.h"

#include "weight_text_item.h"

#include <QPen>
#include <QFont>
#include <QKeyEvent>

QGraphicsEdge::QGraphicsEdge(WeightedEdge *edge, QGraphicsItem *parent)
    : QGraphicsPathItem(parent),
      _edge(edge),
      _weightItem(new WeightEdgeTextItem(QString::number(_edge->weight()), this))
{
    _edge->setGraphicsEdge(this);
    setZValue(0.);

    QPen pen;
    pen.setWidth(2);
    setPen(pen);

    _weightItem->hide();

    connect(_weightItem, SIGNAL(textChanged(int)), this, SLOT(setWeight(int)));
}

QGraphicsEdge::~QGraphicsEdge()
{
    _edge->setGraphicsEdge(nullptr);
}

void QGraphicsEdge::deleteCompletely()
{
    _edge->remove();
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

void QGraphicsEdge::setBrush(const QBrush &brush)
{
    QGraphicsPathItem::setBrush(brush);
    _weightItem->setBrush(brush);
}

QFont QGraphicsEdge::font() const
{
    return _weightItem->font();
}

QBrush QGraphicsEdge::brush() const
{
    return QGraphicsPathItem::brush();
}

void QGraphicsEdge::join(QGraphicsNode *fromNode, QGraphicsNode *toNode)
{
    _edge->setNodes(fromNode->node(), toNode->node());
    refresh();
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

void QGraphicsEdge::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void QGraphicsEdge::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete)
        deleteCompletely();
}

void QGraphicsEdge::showWeight()
{
    _weightItem->show();
}

void QGraphicsEdge::setWeight(int weight)
{
    _edge->setWeight(weight);
    _weightItem->placeInCenter();
}
