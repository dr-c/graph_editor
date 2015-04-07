#ifndef EDGE_H
#define EDGE_H

#include <assert.h>

class Graph;
class Node;

template<typename N, typename E>
class Edge : public E
{
public:
    template<typename... Params>
    Edge(Graph *graph, Params&&... params) : E(params...), _graph(graph), _fromNode(nullptr), _toNode(nullptr) {}

    ~Edge() {
        _fromNode->removeSuccessor(_toNode);
        _toNode->removePredecessor(_fromNode);
    }

    void setNodes(Node<N, E> *fromNode, Node<N, E> *toNode) {
        assert(_fromNode == nullptr && _toNode == nullptr); // Prohibited to reassign
        _fromNode = fromNode;
        _toNode = toNode;
    }

    void remove() {
        _graph->remove(this);
    }

    Node<N, E> *fromNode() const {
        return _fromNode;
    }

    Node<N, E> *toNode() const {
        return _toNode;
    }
    //Graph *graph() const { return _graph; }

private:
    Graph *_graph;
    Node<N, E> *_fromNode;
    Node<N, E> *_toNode;
};

#endif // EDGE_H
