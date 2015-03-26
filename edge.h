#ifndef EDGE_H
#define EDGE_H

class Graph;
class Node;

class Edge
{
public:
    virtual ~Edge();

    void setWeight(int weight);

    Graph   *graph()    const;
    int      weight()   const;
    Node    *nodeFrom() const;
    Node    *nodeTo()   const;

protected:
    Edge(Graph *graph, int weight, Node *from, Node *to);

    Graph   *_graph;
    int      _weight;
    Node    *_from;
    Node    *_to;
};

class DirectedEdge : public Edge
{
public:
    DirectedEdge(Graph *graph, int weight, Node *from, Node *to);
    virtual ~DirectedEdge();
};

class UndirectedEdge : public Edge
{
public:
    UndirectedEdge(Graph *graph, int weight, Node *from, Node *to);
    virtual ~UndirectedEdge();
};

#endif // EDGE_H
