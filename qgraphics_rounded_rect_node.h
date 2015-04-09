#ifndef QGRAPHICSROUNDEDRECTNODE_H
#define QGRAPHICSROUNDEDRECTNODE_H

#include "qgraphics_node.h"

#include <QPainter>

class WeightTextItem;

class QGraphicsRoundedRectNode : public QGraphicsNode
{
    Q_OBJECT
public:
    enum { Type = UserType + 102 };

    QGraphicsRoundedRectNode(WeightedNode *node, qreal lineShiftCoef = 0.8, qreal roundingCoef = 0.2, QGraphicsItem *parent = 0);
    virtual ~QGraphicsRoundedRectNode() override;

    virtual int type() const override;
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
    virtual QPointF calcIntermediatePoint(const QPointF &toPoint) override;

    virtual void setPen(const QPen &pen) override;
    virtual void setFont(const QFont &font, const QColor &color = Qt::black) override;
    virtual void setBrush(const QBrush &brush) override;

    virtual QPen pen() const override;
    virtual QFont font() const override;
    virtual QBrush brush() const override;

    void setCoefficients(qreal lineShiftCoef, qreal roundingCoef);

protected:
    virtual void setGeometry(const QPointF &centerPos) override;
    virtual void calcRadius(int weight) override;

private:
    static qreal checkInRange(qreal var, qreal from, qreal to);

    WeightTextItem          *_weightItem;
    QGraphicsLineItem       *_lineItem;
    QGraphicsSimpleTextItem *_idItem;

    QPen    _pen;
    QBrush  _brush;

    qreal   _lineShiftCoefficient;
    qreal   _roundingCoefficient;
    qreal   _roundingRadius;
};

#endif // QGRAPHICSROUNDEDRECTNODE_H
