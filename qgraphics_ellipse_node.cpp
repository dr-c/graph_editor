#include "qgraphics_ellipse_node.h"

#include "weight_text_item.h"
#include "qgraphics_edge.h"

#include <QPainter>

QGraphicsEllipseNode::QGraphicsEllipseNode(GraphScene *scene, WeightedNode *node, QGraphicsItem *parent)
    : QGraphicsNode(scene, node, parent),
      _ellipseItem(this),
      _lineItem(this)
{
    setGeometry(_node->pos());
}

int QGraphicsEllipseNode::type() const
{
    return Type;
}

QRectF QGraphicsEllipseNode::boundingRect() const
{
    return _ellipseItem.boundingRect();
}

void QGraphicsEllipseNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (this->isSelected())
    {
        QPen pen;
        const qreal halfPenWidth = pen.widthF() / 2;
        pen.setStyle(Qt::DashLine);
        painter->setPen(pen);
        QRectF rect = boundingRect();
        rect.setRect(rect.left() - halfPenWidth, rect.top() - halfPenWidth,
                     rect.width() + pen.widthF(), rect.height() + pen.widthF());
        painter->drawEllipse(rect);
    }
}

void QGraphicsEllipseNode::setFont(const QFont &font, const QColor &color)
{
    _weightItem.setDefaultTextColor(color);
    _weightItem.setFont(font);
    _idItem.setFont(font);
    setGeometry(_node->pos());
}

void QGraphicsEllipseNode::setBrush(const QBrush &brush)
{
    _ellipseItem.setBrush(brush);
}

QFont QGraphicsEllipseNode::font() const
{
    return _idItem.font();
}

QBrush QGraphicsEllipseNode::brush() const
{
    return _ellipseItem.brush();
}

bool QGraphicsEllipseNode::intersects(QGraphicsNode *gNode) const
{
    QPointF difference = _node->pos() - gNode->node()->pos();
    qreal required_distance = _radius + gNode->radius();
    return difference.x() * difference.x() + difference.y() * difference.y() <= required_distance * required_distance;
}

QPainterPath QGraphicsEllipseNode::shape() const
{
    return _ellipseItem.shape();
}

void QGraphicsEllipseNode::setGeometry(const QPointF &centerPos)
{
    const qreal sqrt2 = sqrt(2);
    setPos(centerPos.x() - _radius, centerPos.y() - _radius);

    const qreal diameter = _radius * 2;
    _ellipseItem.setRect(0, 0, diameter, diameter);

    const qreal shift_to_text_item_center = _radius / sqrt2 / 2;
    const QRectF weight_item_rect = _weightItem.boundingRect();
    _weightItem.setPos(_radius + shift_to_text_item_center - weight_item_rect.width() / 2,
                       _radius + shift_to_text_item_center - weight_item_rect.height() / 2);

    const qreal line_item_rect_shift = _radius - _radius / sqrt2;
    _lineItem.setPos(line_item_rect_shift, line_item_rect_shift);
    const qreal line_projection_length = _radius * sqrt2;
    _lineItem.setLine(0, line_projection_length, line_projection_length, 0);

    const QRectF id_item_rect = _idItem.boundingRect();
    _idItem.setPos(_radius - shift_to_text_item_center - id_item_rect.width() / 2,
                   _radius - shift_to_text_item_center - id_item_rect.height() / 2);
}

void QGraphicsEllipseNode::setActivePen(const QPen &pen)
{
    _ellipseItem.setPen(pen);
    _lineItem.setPen(pen);
}

QPointF QGraphicsEllipseNode::calcIntermediatePoint(const QPointF &toPoint) const
{
    return QGraphicsEdge::calcIntermediatePoint(_node->pos(), toPoint, _radius);
}
