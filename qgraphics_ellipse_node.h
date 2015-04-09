#ifndef QGRAPHICSELLIPSENODE_H
#define QGRAPHICSELLIPSENODE_H

#include "qgraphics_node.h"

class WeightTextItem;

class QGraphicsEllipseNode : public QGraphicsNode
{
    Q_OBJECT
public:
    enum { Type = UserType + 101 };

    QGraphicsEllipseNode(WeightedNode *node, QGraphicsItem *parent = 0);
    virtual ~QGraphicsEllipseNode() override;

    virtual int type() const override;
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

    virtual void setPen(const QPen &pen) override;
    virtual void setFont(const QFont &font, const QColor &color = Qt::black) override;
    virtual void setBrush(const QBrush &brush) override;

    virtual QPen pen() const override;
    virtual QFont font() const override;
    virtual QBrush brush() const override;

protected:
    virtual void setGeometry(const QPointF &centerPos);

private:
    QGraphicsEllipseItem    *_ellipseItem;
    WeightTextItem          *_weightItem;
    QGraphicsLineItem       *_lineItem;
    QGraphicsSimpleTextItem *_idItem;
};

#endif // QGRAPHICSELLIPSENODE_H
