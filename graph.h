#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <assert.h>

class Node;
class DirectedNode;
class UndirectedNode;
class Edge;

template<typename N, typename E>
class Graph
{
public:
    virtual ~Graph() {
        for (auto edge : _edges)
            delete edge;
        _edges.clear();
        for (auto node : _nodes)
            delete node;
        _nodes.clear();
    }

    template<typename... Params>
    virtual Node<N, E> *createNode(Params&&... params) = 0;

    template<typename... Params>
    Edge<N, E> *createEdge(Params&&... params) {
        auto edge = new Edge<N, E>(this, params);
        _edges.push_back(edge);
        return edge;
    }

    void remove(Node<N, E> *node) {
        auto erased_count = _nodes.erase(node->id());
        assert(erased_count == 1);
        delete node;
    }

    void remove(Edge<N, E> *edge) {
        auto erased_count = _edges.erase(edge);
        assert(erased_count == 1);
        delete edge;
    }

    const std::map<int, Node*> &nodes() const {
        return _nodes;
    }

    const std::set<Edge*> &edges() const {
        return _edges;
    }

protected:
    Graph() {}

    std::map<int, Node*>    _nodes;
    std::vector<Edge*>      _edges;
};

template<typename N, typename E>
class DirectedGraph : public Graph<N, E>
{
public:
    DirectedGraph() : Graph<N, E>() {}
    virtual ~DirectedGraph() override {}

    template<typename... Params>
    virtual Node<N, E> *createNode(Params&&... params) override {
        auto node = new DirectedNode<N, E>(this, id, params...);
        auto pair = _nodes.insert(make_pair(_nodes.rbegin()->first, node));
        assert(pair.second == true);
        return node;
    }
};

template<typename N, typename E>
class UndirectedGraph : public Graph<N, E>
{
public:
    UndirectedGraph() : Graph<N, E>() {}
    virtual ~UndirectedGraph() override {}

    template<typename... Params>
    virtual Node<N, E> *createNode(Params&&... params) override {
        auto node = new UndirectedNode<N, E>(this, id, params...);
        auto pair = _nodes.insert(make_pair(_nodes.rbegin()->first, node));
        assert(pair.second == true);
        return node;
    }
};

#endif // GRAPH_H
