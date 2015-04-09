#ifndef QGRAPHICSSIMPLELINEEDGE_H
#define QGRAPHICSSIMPLELINEEDGE_H

#include "qgraphics_edge.h"

class QGraphicsSimpleLineEdge : public QGraphicsEdge
{
public:
    enum { Type = UserType + 201 };

    QGraphicsSimpleLineEdge(WeightedEdge *edge, QGraphicsItem *parent = 0);
    virtual ~QGraphicsSimpleLineEdge();

    virtual int type() const override;
    virtual void draw(QGraphicsNode *fromNode, QGraphicsNode *toNode) override;
    virtual void draw(QGraphicsNode *fromNode, const QPointF &toPoint) override;

private:
    static QPointF calcIntermediatePoint(const QPointF &fromPoint, const QPointF &toPoint, qreal bCathetus);
};

#endif // QGRAPHICSSIMPLELINEEDGE_H
