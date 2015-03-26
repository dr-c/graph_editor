#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

class Node;
class Edge;
class QPointF;

class Graph
{
public:
    virtual ~Graph();

    virtual Node *createNode(int id, int weight, const QPointF &pos) = 0;
    virtual Edge *createEdge(int weight, Node *from, Node *to) = 0;

    const std::vector<Node*> &nodes() const;
    const std::vector<Edge*> &edges() const;

protected:
    Graph();

    std::vector<Node*>  _nodes;
    std::vector<Edge*>  _edges;

private:
    std::vector<int>    _id_nodes;
};

class DirectedGraph : public Graph
{
public:
    DirectedGraph();
    virtual ~DirectedGraph();

    virtual Node *createNode(int id, int weight, const QPointF &pos);
    virtual Edge *createEdge(int weight, Node *from, Node *to);
};

class UndirectedGraph : public Graph
{
public:
    UndirectedGraph();
    virtual ~UndirectedGraph();

    virtual Node *createNode(int id, int weight, const QPointF &pos);
    virtual Edge *createEdge(int weight, Node *from, Node *to);
};

#endif // GRAPH_H
