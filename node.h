#ifndef NODE_H
#define NODE_H

#include <map>
#include <algorithm>
#include <assert.h>

template<typename N, typename E>
class Node;

#include "graph.h"
#include "edge.h"

template<typename N, typename E>
class Node : public N
{
    friend Node<N, E> *Graph<N, E>::createNode();
    friend void Graph<N, E>::remove(Node<N, E> *node);
    friend Graph<N, E>::~Graph<N, E>();

public:
    typedef std::map<Node<N, E>*, Edge<N, E>*>  map;
    typedef const map&  map_ref;

    virtual void    addSuccessor(Node<N, E> *node, Edge<N, E> *edge) = 0;
    virtual void    removeSuccessor(Node<N, E> *node) = 0;
    virtual bool    hasSuccessor(Node<N, E> *node) const = 0;
    virtual map_ref successors() const = 0;

    virtual void    addPredecessor(Node<N, E> *node, Edge<N, E> *edge) = 0;
    virtual void    removePredecessor(Node<N, E> *node) = 0;
    virtual bool    hasPredecessor(Node<N, E> *node) const = 0;
    virtual map_ref predecessors() const = 0;

    virtual void    for_each(void (*funct)(std::pair<Node<N, E>* const, Edge<N, E>*>&)) = 0;

    void remove() {
        _graph->remove(this);
    }

    int id() const {
        return _id;
    }

    Graph<N, E> *graph() const {
        return _graph;
    }

protected:
    Node(Graph<N, E> *graph, int id) : N(), _graph(graph), _id(id) {}
    Node(const Node<N, E> &node) = delete;
    Node<N, E> &operator=(const Node<N, E> &node) = delete;
    virtual ~Node() override = default;

private:
    Graph<N, E> *_graph;
    int _id;
};

template<typename N, typename E>
class DirectedNode : public Node<N, E>
{
    friend Node<N, E> *DirectedGraph<N, E>::createNode();

    using typename Node<N, E>::map;
    using typename Node<N, E>::map_ref;

public:
    virtual void addSuccessor(Node<N, E> *node, Edge<N, E> *edge) override {
        assert(_successors.find(node) == _successors.end());
        _successors.insert(std::make_pair(node, edge));
    }

    virtual void removeSuccessor(Node<N, E> *node) override {
        assert(_successors.find(node) != _successors.end());
        _successors.erase(node);
    }

    virtual bool hasSuccessor(Node<N, E> *node) const override {
        return _successors.find(node) != _successors.end();
    }

    virtual map_ref successors() const override {
        return _successors;
    }

    virtual void addPredecessor(Node<N, E> *node, Edge<N, E> *edge) override {
        assert(_predecessors.find(node) == _predecessors.end());
        _predecessors.insert(std::make_pair(node, edge));
    }

    virtual void removePredecessor(Node<N, E> *node) override {
        assert(_predecessors.find(node) != _predecessors.end());
        _predecessors.erase(node);
    }

    virtual bool hasPredecessor(Node<N, E> *node) const override {
        return _predecessors.find(node) != _predecessors.end();
    }

    virtual map_ref predecessors() const override {
        return _predecessors;
    }

    virtual void for_each(void (*funct)(std::pair<Node<N, E>* const, Edge<N, E>*>&)) override {
        std::for_each(_successors.begin(), _successors.end(), funct);
        std::for_each(_predecessors.begin(), _predecessors.end(), funct);
    }

protected:
    DirectedNode(Graph<N, E> *graph, int id) : Node<N, E>(graph, id) {}
    DirectedNode(const DirectedNode<N, E> &node) = delete;
    DirectedNode<N, E> &operator=(const DirectedNode<N, E> &node) = delete;
    virtual ~DirectedNode() override {
        for (auto& pair : _successors)
            pair.second->remove();
        for (auto& pair : _predecessors)
            pair.second->remove();
    }

private:
    map _successors;
    map _predecessors;
};

template<typename N, typename E>
class UndirectedNode : public Node<N, E>
{
    friend Node<N, E> *UndirectedGraph<N, E>::createNode();

    using typename Node<N, E>::map;
    using typename Node<N, E>::map_ref;

public:
    virtual void addSuccessor(Node<N, E> *node, Edge<N, E> *edge) override {
        assert(_adjacents.find(node) == _adjacents.end());
        _adjacents.insert(std::make_pair(node, edge));
    }

    virtual void removeSuccessor(Node<N, E> *node) override {
        assert(_adjacents.find(node) != _adjacents.end());
        _adjacents.erase(node);
    }

    virtual bool hasSuccessor(Node<N, E> *node) const override {
        return _adjacents.find(node) != _adjacents.end();
    }

    virtual map_ref successors() const override {
        return _adjacents;
    }

    virtual void addPredecessor(Node<N, E> *node, Edge<N, E> *edge) override {
        assert(_adjacents.find(node) == _adjacents.end());
        _adjacents.insert(std::make_pair(node, edge));
    }

    virtual void removePredecessor(Node<N, E> *node) override {
        assert(_adjacents.find(node) != _adjacents.end());
        _adjacents.erase(node);
    }

    virtual bool hasPredecessor(Node<N, E> *node) const override {
        return _adjacents.find(node) != _adjacents.end();
    }

    virtual map_ref predecessors() const override {
        return _adjacents;
    }

    virtual void for_each(void (*funct)(std::pair<Node<N, E>* const, Edge<N, E>*>&)) override {
        std::for_each(_adjacents.begin(), _adjacents.end(), funct);
    }

protected:
    UndirectedNode(Graph<N, E> *graph, int id) : Node<N, E>(graph, id) {}
    UndirectedNode(const UndirectedNode<N, E> &node) = delete;
    UndirectedNode<N, E> &operator=(const UndirectedNode<N, E> &node) = delete;
    virtual ~UndirectedNode() override {
        for (auto& pair : _adjacents)
            pair.second->remove();
    }

private:
    map _adjacents;
};

#endif // NODE_H
