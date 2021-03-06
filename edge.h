/*
 * Edge - logical representation of a line, which connects two vertices of graph(nodes).
 *
 * Edge may be constructed only in Graph with the same template parameters.
 * Edge inherites from <E>. It allows to create a different edges easily.
 * <E> must be a class with default constructor and virtual destructor.
 *
 * After creation, Edge is useless until setNodes(...), which connect two Nodes.
 * It's impossible to Reassign Nodes with setNodes(...).
 * <N> specifies type of Nodes, which connected(can be connected) by this Edge.
 * <N> must be a class with default constructor and virtual destructor.
 *
 * For correct deletion, it's necessary to use remove(). Edge will be destroyed, after removing from Graph.
 */

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
    friend class Graph<N, E>;

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
