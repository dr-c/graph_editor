#include "qgraphics_edge.h"

QGraphicsEdge::QGraphicsEdge(WeightedEdge *edge, QGraphicsItem *parent)
    : QGraphicsPathItem(parent), _edge(edge)
{
    _edge->setGraphicsEdge(this);
}

QGraphicsEdge::~QGraphicsEdge()
{

}
