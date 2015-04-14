/*
 * QGraphicsRoundedRectNode - rectangle with rounded corners and line,
 *  which divide it into two different part.
 *
 * The size of each parts depends on lineShiftCoef.
 * lineShiftCoef must be in range [minLineShiftCoef;maxLineShiftCoef].
 *
 * The radii of circles defining the corners specified by roundingCoef.
 * roundingCoef must be in range [minRoundingCoef;maxRoundingCoef].
 *
 * Those coefficient can be set in constructor and via setCoefficients() method.
 */


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

    QGraphicsRoundedRectNode(BasicGraphScene *scene, WeightedNode *node, qreal lineShiftCoef = 0.8, qreal roundingCoef = 0.2, QGraphicsItem *parent = 0);
    virtual ~QGraphicsRoundedRectNode() override;

    virtual int type() const override;
    virtual QPainterPath shape() const override;
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

    virtual QPointF calcIntermediatePoint(const QPointF &toPoint) const override;

    virtual bool intersects(QGraphicsNode *gNode) const override;

    virtual void setActivePen(const QPen &pen) override;

    virtual void setFont(const QFont &font, const QColor &color = Qt::black) override;
    virtual QFont font() const override;

    virtual void setBrush(const QBrush &brush) override;
    virtual QBrush brush() const override;

    void setCoefficients(qreal lineShiftCoef, qreal roundingCoef);

protected:
    virtual void setGeometry(const QPointF &centerPos) override;
    virtual void calcRadius(int weight) override;

private:
    static qreal checkInRange(qreal var, qreal from, qreal to);
    static constexpr double minLineShiftCoef = 0.5;
    static constexpr double maxLineShiftCoef = 1.;
    static constexpr double minRoundingCoef = 0.;
    static constexpr double maxRoundingCoef = 0.3;

    QGraphicsLineItem       *_lineItem;

    qreal   _lineShiftCoefficient;
    qreal   _roundingCoefficient;
    qreal   _roundingRadius;

    QPen    _pen;
    QBrush  _brush;
};

#endif // QGRAPHICSROUNDEDRECTNODE_H
