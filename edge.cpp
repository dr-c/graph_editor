#include "edge.h"

#include "graph.h"
#include "node.h"

Edge::Edge(Graph *graph, int weight, Node *from, Node *to)
    : _graph(graph), _weight(weight), _from(from), _to(to)
{

}

Edge::~Edge()
{

}

void Edge::setWeight(int weight)
{
    _weight = weight;
}

Graph *Edge::graph() const
{
    return _graph;
}

int Edge::weight() const
{
    return _weight;
}

Node *Edge::nodeFrom() const
{
    return _from;
}

Node *Edge::nodeTo() const
{
    return _to;
}

DirectedEdge::DirectedEdge(Graph *graph, int weight, Node *from, Node *to)
    : Edge(graph, weight, from, to)
{

}

DirectedEdge::~DirectedEdge()
{

}

UndirectedEdge::UndirectedEdge(Graph *graph, int weight, Node *from, Node *to)
    : Edge(graph, weight, from, to)
{

}

UndirectedEdge::~UndirectedEdge()
{

}
