#ifndef NODE_H
#define NODE_H

#include <map>
#include <assert.h>

template<typename N, typename E>
class Node;

#include "graph.h"
#include "edge.h"

template<typename N, typename E>
class Node : public N
{
public:
    Node(Graph<N, E> *graph, int id) : N(), _graph(graph), _id(id) {}

    virtual ~Node() override {}

    virtual void addSuccessor(Node<N, E> *node, Edge<N, E> *edge) = 0;
    virtual void removeSuccessor(Node<N, E> *node) = 0;

    virtual void addPredecessor(Node<N, E> *node, Edge<N, E> *edge) = 0;
    virtual void removePredecessor(Node<N, E> *node) = 0;

    void remove() {
        _graph->remove(this);
    }

    int id() const {
        return _id;
    }

private:
    Graph<N, E> *_graph;
    int _id;
};

template<typename N, typename E>
class DirectedNode : public Node<N, E>
{
public:
    DirectedNode(Graph<N, E> *graph, int id) : Node<N, E>(graph, id) {}

    virtual ~DirectedNode() override {
        for (auto& pair : _successors)
            pair.second->remove();
        for (auto& pair : _predecessors)
            pair.second->remove();
    }

    virtual void addSuccessor(Node<N, E> *node, Edge<N, E> *edge) override {
        assert(_successors.find(node) == _successors.end());
        _successors.insert(std::make_pair(node, edge));
    }

    virtual void removeSuccessor(Node<N, E> *node) override {
        assert(_successors.find(node) != _successors.end());
        _successors.erase(node);
    }

    virtual void addPredecessor(Node<N, E> *node, Edge<N, E> *edge) override {
        assert(_predecessors.find(node) == _predecessors.end());
        _predecessors.insert(std::make_pair(node, edge));
    }

    virtual void removePredecessor(Node<N, E> *node) override {
        assert(_predecessors.find(node) != _predecessors.end());
        _predecessors.erase(node);
    }

private:
    std::map<Node<N, E>*, Edge<N, E>*> _successors;
    std::map<Node<N, E>*, Edge<N, E>*> _predecessors;
};

template<typename N, typename E>
class UndirectedNode : public Node<N, E>
{
public:
    UndirectedNode(Graph<N, E> *graph, int id) : Node<N, E>(graph, id) {}

    virtual ~UndirectedNode() override {
        for (auto& pair : _adjacents)
            pair.second->remove();
    }

    virtual void addSuccessor(Node<N, E> *node, Edge<N, E> *edge) override {
        assert(_adjacents.find(node) == _adjacents.end());
        _adjacents.insert(std::make_pair(node, edge));
    }

    virtual void removeSuccessor(Node<N, E> *node) override {
        assert(_adjacents.find(node) != _adjacents.end());
        _adjacents.erase(node);
    }

    virtual void addPredecessor(Node<N, E> *node, Edge<N, E> *edge) override {
        assert(_adjacents.find(node) == _adjacents.end());
        _adjacents.insert(std::make_pair(node, edge));
    }

    virtual void removePredecessor(Node<N, E> *node) override {
        assert(_adjacents.find(node) != _adjacents.end());
        _adjacents.erase(node);
    }

private:
    std::map<Node<N, E>*, Edge<N, E>*> _adjacents;
};

#endif // NODE_H
