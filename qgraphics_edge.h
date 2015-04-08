#ifndef QGRAPHICSEDGE_H
#define QGRAPHICSEDGE_H

#include <QGraphicsObject>

#include "item_info.h"

class QGraphicsEdge : public QGraphicsObject
{
    Q_OBJECT
public:
    virtual ~QGraphicsEdge();

    virtual int type() const = 0;

protected:
    QGraphicsEdge(WeightedNode *edge, QGraphicsItem *parent = 0);

    WeightedEdge *_edge;
};

#endif // QGRAPHICSEDGE_H
