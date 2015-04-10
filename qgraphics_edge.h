#ifndef QGRAPHICSEDGE_H
#define QGRAPHICSEDGE_H

#include <QGraphicsObject>

#include "item_info.h"

class QGraphicsEdge : public QGraphicsPathItem
{
public:
    virtual ~QGraphicsEdge() override;

    virtual int type() const = 0;
    virtual void draw(QGraphicsNode *fromNode, QGraphicsNode *toNode) = 0;
    virtual void draw(QGraphicsNode *fromNode, const QPointF &toPoint) = 0;

    void join(QGraphicsNode *fromNode, QGraphicsNode *toNode);
    void update();

    WeightedEdge *edge() const;

protected:
    QGraphicsEdge(WeightedEdge *edge, QGraphicsItem *parent = 0);

    WeightedEdge *_edge;
};

#endif // QGRAPHICSEDGE_H
