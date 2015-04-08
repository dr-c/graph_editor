#include "qgraphics_simple_line_edge.h"

QGraphicsSimpleLineEdge::QGraphicsSimpleLineEdge()
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
