#ifndef QGRAPHICSSIMPLELINEEDGE_H
#define QGRAPHICSSIMPLELINEEDGE_H

#include "qgraphics_edge.h"

class QGraphicsSimpleLineEdge : public QGraphicsEdge
{
public:
    enum { Type = UserType + 201 };

    QGraphicsSimpleLineEdge(WeightedNode *edge, QGraphicsItem *parent = 0);
    virtual ~QGraphicsSimpleLineEdge();

    virtual int type() const;
};

#endif // QGRAPHICSSIMPLELINEEDGE_H
