/*
 * Node - logical representation of the fundamental unit of which graphs are formed.
 * Node connected to other Nodes via Edges.
 *
 * Node may be constructed only in Graph with the same template parameters.
 * Node inherites from <N>. It allows to create a different nodes easily.
 * <N> must be a class with default constructor and virtual destructor.
 *
 * It`s prohibited to connect Nodes directly. Use Edges(method setNodes).
 * Connect two nodes twice also prohibited.
 *
 * Where exist Edge from Node A to Node B this means, Node B is successor for Node A,
 *  and Node A is predecessor for Node B.
 * In UndirectedNode, if exist a way from Node A to Node B, then exist a reverse way, from B to A.
 * This means, for UndirectedNode list of successors and predecessors is the same. See CAREFUL section.
 *
 * For checking, are two nodes already connected, use methods hasSuccesor(Node*)/hasPredecessor(Node*).
 * For accessing to list of successors/predecessors use successors()/predecessors() methods.
 * Thet return map with pairs of [Node, connected to this]->[Edge connecting these two Nodes].
 *
 * < !!! CAREFUL !!!
 * successors()/predecessors() methods for UndirectedNode return same map.
 * That`s why not recommended to use them for iterating over all ajacent Nodes(associated Edges).
 * For this purpose use method for_each, which described below.
 * !!! CAREFUL !!! >
 *
 * For iterating over all ajacent(not only successors or predecessors) Nodes and/or associated Edges,
 *  use method for_each. It takes a pointer to function, which will execute for each element in
 *   map(s) of ajacent Nodes and associated Edges.
 * Unfortunately, for_each() cann`t take Functor, because virtual template function is deprecated.
 * But, It is simply to use with lamba-functions.
 *
 * It`s possible to compare Nodes via id(). Each Node has a unique id_number.
 *
 * <E> specifies type of Edges, which connects the Node other Nodes.
 * <E> must be a class with default constructor and virtual destructor.
 *
 * For correct deletion, necessary to use remove(). It will destroy Node, after removing from Graph.
 * Node deletion deletes all edges, which connects this Node to other Nodes.
 */

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
    friend class Graph<N, E>;
    friend class Edge<N, E>;

public:
    typedef std::map<Node<N, E>*, Edge<N, E>*>  map;
    typedef const map&  map_ref;

    virtual bool    hasSuccessor(Node<N, E> *node) const = 0;
    virtual map_ref successors() const = 0;

    virtual bool    hasPredecessor(Node<N, E> *node) const = 0;
    virtual map_ref predecessors() const = 0;

    virtual typename map::size_type  countAdjacents() const = 0;

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

    virtual void    addSuccessor(Node<N, E> *node, Edge<N, E> *edge) = 0;
    virtual void    removeSuccessor(Node<N, E> *node) = 0;

    virtual void    addPredecessor(Node<N, E> *node, Edge<N, E> *edge) = 0;
    virtual void    removePredecessor(Node<N, E> *node) = 0;

private:
    Graph<N, E> *_graph;
    int _id;
};

template<typename N, typename E>
class DirectedNode : public Node<N, E>
{
    friend class DirectedGraph<N, E>;
    friend class Edge<N, E>;

    using typename Node<N, E>::map;
    using typename Node<N, E>::map_ref;

public:
    virtual bool hasSuccessor(Node<N, E> *node) const override {
        return _successors.find(node) != _successors.end();
    }

    virtual map_ref successors() const override {
        return _successors;
    }

    virtual bool hasPredecessor(Node<N, E> *node) const override {
        return _predecessors.find(node) != _predecessors.end();
    }

    virtual map_ref predecessors() const override {
        return _predecessors;
    }

    virtual typename map::size_type countAdjacents() const override {
        return _successors.size() + _predecessors.size();
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
        while (!_successors.empty())
            _successors.begin()->second->remove();
        while (!_predecessors.empty())
            _predecessors.begin()->second->remove();
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
    map _successors;
    map _predecessors;
};

template<typename N, typename E>
class UndirectedNode : public Node<N, E>
{
    friend class UndirectedGraph<N, E>;
    friend class Edge<N, E>;

    using typename Node<N, E>::map;
    using typename Node<N, E>::map_ref;

public:
    virtual bool hasSuccessor(Node<N, E> *node) const override {
        return _adjacents.find(node) != _adjacents.end();
    }

    virtual map_ref successors() const override {
        return _adjacents;
    }

    virtual bool hasPredecessor(Node<N, E> *node) const override {
        return _adjacents.find(node) != _adjacents.end();
    }

    virtual map_ref predecessors() const override {
        return _adjacents;
    }

    virtual typename map::size_type countAdjacents() const override {
        return _adjacents.size();
    }

    virtual void for_each(void (*funct)(std::pair<Node<N, E>* const, Edge<N, E>*>&)) override {
        std::for_each(_adjacents.begin(), _adjacents.end(), funct);
    }

protected:
    UndirectedNode(Graph<N, E> *graph, int id) : Node<N, E>(graph, id) {}
    UndirectedNode(const UndirectedNode<N, E> &node) = delete;
    UndirectedNode<N, E> &operator=(const UndirectedNode<N, E> &node) = delete;
    virtual ~UndirectedNode() override {
        while (!_adjacents.empty())
            _adjacents.begin()->second->remove();
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
    map _adjacents;
};

#endif // NODE_H
