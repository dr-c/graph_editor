#include "qgraphics_edge.h"

QGraphicsEdge::QGraphicsEdge(WeightedNode *edge, QGraphicsItem *parent)
    : QGraphicsObject(parent), _edge(edge)
{

}

QGraphicsEdge::~QGraphicsEdge()
{

}
