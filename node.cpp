#include "node.h"

#include "graph.h"
#include "edge.h"

Node::Node(Graph *graph, int id, int weight, const QPointF &pos)
    : _graph(graph), _id(id), _weight(weight), _pos(pos)
{

}

Node::~Node()
{

}

void Node::setWeight(int weight)
{
    _weight = weight;
}

void Node::setPos(const QPointF &pos)
{
    _pos = pos;
}

int Node::id() const
{
    return _id;
}

int Node::weight() const
{
    return _weight;
}

QPointF Node::pos() const
{
    return _pos;
}
