#ifndef EDGE_H
#define EDGE_H

#include <assert.h>

template<typename N, typename E>
class Edge;

#include "graph.h"
#include "node.h"

template<typename N, typename E>
class Edge : public E
{
    friend Edge<N, E> *Graph<N, E>::createEdge();
    friend void Graph<N, E>::remove(Edge<N, E> *edge);
    friend Graph<N, E>::~Graph<N, E>();

public:
    void setNodes(Node<N, E> *fromNode, Node<N, E> *toNode) {
        assert(_fromNode == nullptr && _toNode == nullptr); // Prohibited to reassign
        assert(fromNode != nullptr && toNode != nullptr);   // Prohibited to assign nullptr
        _fromNode = fromNode;
        _toNode = toNode;
        _fromNode->addSuccessor(_toNode, this);
        _toNode->addPredecessor(_fromNode, this);
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

    Graph<N, E> *graph() const {
        return _graph;
    }

protected:
    Edge(Graph<N, E> *graph) : E(), _graph(graph), _fromNode(nullptr), _toNode(nullptr) {}
    Edge(const Edge<N, E> &edge) = delete;
    Edge<N, E> &operator=(const Edge<N, E> &edge) = delete;
    virtual ~Edge() override {
        if (_fromNode != nullptr) {
            _fromNode->removeSuccessor(_toNode);
            _toNode->removePredecessor(_fromNode);
        }
    }

private:
    Graph<N, E> *_graph;
    Node<N, E>  *_fromNode;
    Node<N, E>  *_toNode;
};

#endif // EDGE_H
