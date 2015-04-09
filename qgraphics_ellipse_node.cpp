#include "qgraphics_ellipse_node.h"

#include "weight_text_item.h"

#include <QPainter>

QGraphicsEllipseNode::QGraphicsEllipseNode(WeightedNode *node, QGraphicsItem *parent)
    : QGraphicsNode(node, parent),
      _ellipseItem(new QGraphicsEllipseItem(this)),
      _weightItem(new WeightTextItem(_node->weight(), this)),
      _lineItem(new QGraphicsLineItem(this)),
      _idItem(new QGraphicsSimpleTextItem(QString::number(_node->id()), this))
{
    setGeometry(_node->pos());

    connect(_weightItem, SIGNAL(textChanged(int)), this, SLOT(setWeight(int)));
}

QGraphicsEllipseNode::~QGraphicsEllipseNode()
{
    delete _idItem;
    delete _lineItem;
    delete _weightItem;
    delete _ellipseItem;
}

int QGraphicsEllipseNode::type() const
{
    return Type;
}

QRectF QGraphicsEllipseNode::boundingRect() const
{
    return _ellipseItem->boundingRect();
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

void QGraphicsEllipseNode::setPen(const QPen &pen)
{
    _ellipseItem->setPen(pen);
    _lineItem->setPen(pen);
}

void QGraphicsEllipseNode::setFont(const QFont &font, const QColor &color)
{
    _weightItem->setDefaultTextColor(color);
    _weightItem->setFont(font);
    _idItem->setFont(font);
    setGeometry(_node->pos());
}

void QGraphicsEllipseNode::setBrush(const QBrush &brush)
{
    _ellipseItem->setBrush(brush);
}

QPen QGraphicsEllipseNode::pen() const
{
    return _ellipseItem->pen();
}

QFont QGraphicsEllipseNode::font() const
{
    return _idItem->font();
}

QBrush QGraphicsEllipseNode::brush() const
{
    return _ellipseItem->brush();
}

void QGraphicsEllipseNode::setGeometry(const QPointF &centerPos)
{
    const qreal sqrt2 = sqrt(2);
    setPos(centerPos.x() - _radius, centerPos.y() - _radius);

    const qreal diameter = _radius * 2;
    _ellipseItem->setRect(0, 0, diameter, diameter);

    const qreal shift_to_text_item_center = _radius / sqrt2 / 2;
    const QRectF weight_item_rect = _weightItem->boundingRect();
    _weightItem->setPos(_radius + shift_to_text_item_center - weight_item_rect.width() / 2,
                       _radius + shift_to_text_item_center - weight_item_rect.height() / 2);

    const qreal line_item_rect_shift = _radius - _radius / sqrt2;
    _lineItem->setPos(line_item_rect_shift, line_item_rect_shift);
    const qreal line_projection_length = _radius * sqrt2;
    _lineItem->setLine(0, line_projection_length, line_projection_length, 0);

    const QRectF id_item_rect = _idItem->boundingRect();
    _idItem->setPos(_radius - shift_to_text_item_center - id_item_rect.width() / 2,
                    _radius - shift_to_text_item_center - id_item_rect.height() / 2);
}
