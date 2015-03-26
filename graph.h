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

protected:
    Graph();
};

class DirectedGraph : public Graph
{
public:
    DirectedGraph();
    virtual ~DirectedGraph();
};

class UndirectedGraph : public Graph
{
public:
    UndirectedGraph();
    virtual ~UndirectedGraph();
};

#endif // GRAPH_H
