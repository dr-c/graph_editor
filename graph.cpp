#include "graph.h"

#include "node.h"
#include "edge.h"

Graph::Graph()
{

}

Graph::~Graph()
{
    while (!_nodes.empty())
        delete _nodes.back();
    while (!_edges.empty())
        delete _edges.back();
}

const std::vector<Node*> &Graph::nodes() const
{
    return _nodes;
}

const std::vector<Edge*> &Graph::edges() const
{
    return _edges;
}

DirectedGraph::DirectedGraph()
    : Graph()
{

}

DirectedGraph::~DirectedGraph()
{

}

Node *DirectedGraph::createNode(int id, int weight, const QPointF &pos)
{
    Node *node = new Node(this, id, weight, pos);
    _nodes.push_back(node);
    return node;
}

Edge *DirectedGraph::createEdge(int weight, Node *from, Node *to)
{
    Edge *edge = new DirectedEdge(this, weight, from, to);
    _edges.push_back(edge);
    return edge;
}

UndirectedGraph::UndirectedGraph()
    : Graph()
{

}

UndirectedGraph::~UndirectedGraph()
{

}

Node *UndirectedGraph::createNode(int id, int weight, const QPointF &pos)
{
    Node *node = new Node(this, id, weight, pos);
    _nodes.push_back(node);
    return node;
}

Edge *UndirectedGraph::createEdge(int weight, Node *from, Node *to)
{
    Edge *edge = new UndirectedEdge(this, weight, from, to);
    _edges.push_back(edge);
    return edge;
}
