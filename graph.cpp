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

Node *Graph::createNode(int id, int weight, const QPointF &pos)
{
    auto low_iter = std::lower_bound(_id_nodes.begin(), _id_nodes.end(), id);
    // if id is busy by another Node
    if (low_iter != _id_nodes.end() && *low_iter == id)
    {
        id = _id_nodes.back() + 1;
        low_iter = _id_nodes.end();
    }

    Node *node = new Node(this, id, weight, pos);
    _nodes.push_back(node);
    _id_nodes.insert(low_iter, id);
    return node;
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

Edge *UndirectedGraph::createEdge(int weight, Node *from, Node *to)
{
    Edge *edge = new UndirectedEdge(this, weight, from, to);
    _edges.push_back(edge);
    return edge;
}
