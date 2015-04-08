#include "qgraphics_edge.h"

QGraphicsEdge::QGraphicsEdge(WeightedEdge *edge, QGraphicsItem *parent)
    : QGraphicsPathItem(parent), _edge(edge)
{

}

QGraphicsEdge::~QGraphicsEdge()
{

}
