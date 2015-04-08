#include "qgraphics_simple_line_edge.h"

QGraphicsSimpleLineEdge::QGraphicsSimpleLineEdge(WeightedEdge *edge, QGraphicsItem *parent)
    : QGraphicsEdge(edge, parent)
{

}

QGraphicsSimpleLineEdge::~QGraphicsSimpleLineEdge()
{

}

int QGraphicsSimpleLineEdge::type() const
{
    return Type;
}
