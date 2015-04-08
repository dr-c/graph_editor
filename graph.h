#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <assert.h>

template<typename N, typename E>
class Graph;

#include "node.h"
#include "edge.h"

template<typename N, typename E>
class Graph
{
public:
    virtual ~Graph() {
        for (auto edge : _edges)
            delete edge;
        _edges.clear();
        for (auto& node : _nodes)
            delete node.second;
        _nodes.clear();
    }

    virtual Node<N, E> *createNode() = 0;

    Edge<N, E> *createEdge() {
        auto edge = new Edge<N, E>(this);
        _edges.push_back(edge);
        return edge;
    }

    void remove(Node<N, E> *node) {
        auto erased_count = _nodes.erase(node->id());
        assert(erased_count == 1);
        delete node;
    }

    void remove(Edge<N, E> *edge) {
        auto iter = _edges.begin();
        while (iter != _edges.end()) {
            if (*iter == edge)
                break;
            ++iter;
        }
        assert(iter != _edges.end());
        _edges.erase(iter);
        delete edge;
    }

    const std::map<int, Node<N, E>*> &nodes() const {
        return _nodes;
    }

    const std::vector<Edge<N, E>*> &edges() const {
        return _edges;
    }

protected:
    Graph() {}

    std::map<int, Node<N, E>*>    _nodes;
    std::vector<Edge<N, E>*>      _edges;
};

template<typename N, typename E>
class DirectedGraph : public Graph<N, E>
{
public:
    DirectedGraph() : Graph<N, E>() {}
    virtual ~DirectedGraph() override {}

    virtual Node<N, E> *createNode() override {
        int id = this->_nodes.empty() ? 1 : this->_nodes.rbegin()->first + 1;
        auto node = new DirectedNode<N, E>(this, id);
        auto pair = this->_nodes.insert(std::make_pair(id, node));
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

    virtual Node<N, E> *createNode() override {
        int id = this->_nodes.rbegin()->first;
        auto node = new UndirectedNode<N, E>(this, id);
        auto pair = this->_nodes.insert(std::make_pair(id, node));
        assert(pair.second == true);
        return node;
    }
};

#endif // GRAPH_H
