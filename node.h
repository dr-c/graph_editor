#ifndef NODE_H
#define NODE_H

#include <map>
//#include <type_traits>
#include <assert.h>
//#include <algorithm>

class Graph;
class Edge;

template<typename N, typename E>
class Node : public N
{
public:
    template<typename... Params>
    Node(Graph<N, E> *graph, int id, Params&&... params) : N(params...), _graph(graph), _id(id) {
        //static_assert not necessary, because destructor has override specifier
        //static_assert(std::has_virtual_destructor<N>::value, "First template parameter N hasn't virtual destructor");
    }

    virtual ~Node() override {}

    virtual void addSuccessor(Node<N> *node, Edge<E> *edge) = 0;
    virtual void removeSuccessor(Node<N> *node) = 0;

    virtual void addPredecessor(Node<N> *node, Edge<E> *edge) = 0;
    virtual void removePredecessor(Node<N> *node) = 0;

    //template<typename Fnctr>
    //virtual void for_each(Fnctr f) = 0;

    void remove() {
        _graph->remove(this);
    }

    //Graph *graph() const { return _graph; }
    int id() const {
        return _id;
    }

private:
    Graph<N, E> *_graph;
    int _id;
};

template<typename N, typename E>
class DirectedNode : public Node<N>
{
public:
    template<typename... Params>
    DirectedNode(Graph<N, E> *graph, int id, Params&&... params) : Node<N, E>(graph, id, params...) {}

    virtual ~DirectedNode() override {
        for (auto edge : _successors)
            edge->remove();
        for (auto edge : _predecessors)
            edge->remove();
    }

    virtual void addSuccessor(Node<N> *node, Edge<E> *edge) override {
        assert(_successors.find(node) == _successors.end());
        _successors.insert(node, edge);
    }

    virtual void removeSuccessor(Node<N> *node) override {
        assert(_successors.find(node) != _successors.end());
        _successors.erase(node);
    }

    virtual void addPredecessor(Node<N> *node, Edge<E> *edge) override {
        assert(_predecessors.find(node) == _predecessors.end());
        _predecessors.insert(node, edge);
    }

    virtual void removePredecessor(Node<N> *node) override {
        assert(_predecessors.find(node) != _predecessors.end());
        _predecessors.erase(node);
    }

    //template<typename Fnctr>
    //virtual void for_each(Fnctr f)
    //{
    //    static_assert(std::is_function<Fnctr>::value, "Fnctr f is not a functor");
    //    std::for_each(_successors.begin(), _successors.end(), f);
    //    std::for_each(_predecessors.begin(), _predecessors.end(), f);
    //}

private:
    std::map<Node<N>*, Edge<E>*> _successors;
    std::map<Node<N>*, Edge<E>*> _predecessors;
};

template<typename N, typename E>
class UndirectedNode : public Node<N>
{
public:
    template<typename... Params>
    UndirectedNode(Graph<N, E> *graph, int id, Params... params) : Node<N, E>(graph, id, params...) {}

    virtual ~UndirectedNode() override {
        for (auto edge : _adjacents)
            edge->remove();
    }

    virtual void addSuccessor(Node<N> *node, Edge<E> *edge) override {
        assert(_adjacents.find(node) == _adjacents.end());
        _adjacents.insert(node, edge);
    }

    virtual void removeSuccessor(Node<N> *node) override {
        assert(_adjacents.find(node) != _adjacents.end());
        _adjacents.erase(node);
    }

    virtual void addPredecessor(Node<N> *node, Edge<E> *edge) override {
        assert(_adjacents.find(node) == _adjacents.end());
        _adjacents.insert(node, edge);
    }

    virtual void removePredecessor(Node<N> *node) override {
        assert(_adjacents.find(node) != _adjacents.end());
        _adjacents.erase(node);
    }

    //template<typename Fnctr>
    //virtual void for_each(Fnctr f)
    //{
    //    static_assert(std::is_function<Fnctr>::value, "Fnctr f is not a functor");
    //    std::for_each(_adjacents.begin(), _adjacents.end(), f);
    //}

private:
    std::map<Node<N>*, Edge<E>*> _adjacents;
};

#endif // NODE_H
