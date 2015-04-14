/*
 * QGraphicsSimpleLineEdge - just line.
 *
 * QGraphicsSimpleLineEdge one of the implementation of QGraphicsEdge.
 * It is best to use QGraphicsCubicArrowEdge with UndirectedGraph, as UndirectedEdge.
 */

#ifndef QGRAPHICSSIMPLELINEEDGE_H
#define QGRAPHICSSIMPLELINEEDGE_H

#include "qgraphics_edge.h"

class QGraphicsSimpleLineEdge : public QGraphicsEdge
{
public:
    enum { Type = UserType + 201 };

    QGraphicsSimpleLineEdge(BasicGraphScene *scene, WeightedEdge *edge, QGraphicsItem *parent = 0);
    virtual ~QGraphicsSimpleLineEdge() override = default;

    virtual int type() const override;
    virtual void draw(QGraphicsNode *fromGNode, QGraphicsNode *toGNode) override;
    virtual void draw(QGraphicsNode *fromGNode, const QPointF &toPoint) override;
};

#endif // QGRAPHICSSIMPLELINEEDGE_H
