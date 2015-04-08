#ifndef QGRAPHICSEDGE_H
#define QGRAPHICSEDGE_H

#include <QGraphicsObject>

#include "item_info.h"

class QGraphicsEdge : public QGraphicsPathItem
{
public:
    virtual ~QGraphicsEdge();

    virtual int type() const = 0;

protected:
    QGraphicsEdge(WeightedEdge *edge, QGraphicsItem *parent = 0);

    WeightedEdge *_edge;
};

#endif // QGRAPHICSEDGE_H
