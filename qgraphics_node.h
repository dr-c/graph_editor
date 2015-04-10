#ifndef QGRAPHICSNODE_H
#define QGRAPHICSNODE_H

#include <QGraphicsObject>

#include "item_info.h"

class WeightTextItem;

class QGraphicsNode : public QGraphicsObject
{
    Q_OBJECT
public:
    virtual ~QGraphicsNode() override;

    virtual int type() const = 0;
    virtual QRectF boundingRect() const = 0;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) = 0;
    virtual QPointF calcIntermediatePoint(const QPointF &toPoint) = 0;

    virtual void setPen(const QPen &pen) = 0;
    virtual void setFont(const QFont &font, const QColor &color = Qt::black) = 0;
    virtual void setBrush(const QBrush &brush) = 0;

    virtual QPen pen() const = 0;
    virtual QFont font() const = 0;
    virtual QBrush brush() const = 0;

    qreal radius() const;
    WeightedNode *node() const;

protected slots:
    void setWeight(int weight);

protected:
    QGraphicsNode(WeightedNode *node, QGraphicsItem *parent = 0);

    virtual void        deleteCompletely();

    virtual void        setGeometry(const QPointF &centerPos) = 0;
    virtual void        calcRadius(int weight);

    virtual void        mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void        keyPressEvent(QKeyEvent *event) override;
    virtual QVariant    itemChange(GraphicsItemChange change, const QVariant &value) override;

    WeightedNode *_node;

    WeightTextItem          *_weightItem;
    QGraphicsSimpleTextItem *_idItem;

    qreal _radius;

private:
    void updateRelatedEdges();

    static  const       int     RADIUS_SHIFT = 25;
    static  constexpr   qreal   RADIUS_SCALE = 5.;
};

#endif // QGRAPHICSNODE_H
