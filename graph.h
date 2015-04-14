/*
 * Graph - logical representation of a set of objects(nodes),
 *  where some objects are connected by links (edges).
 *
 * Graph stores two sets: Nodes and Edges, which was created by means createNode()/createEdge() methods.
 * This sets can be accessible via nodes() and edges() respectively.
 *
 * Graph is abstract factory for Nodes and Edges.
 * It provide functionality not only for creating(createNode()/createEdge()),
 *  but also and deleting(remove(Node*)/remove(Edge*)) Nodes and Edges.
 * Deleting may be accomplished from Node/edge directly via remove(). It`s actualy the same.
 *
 * Template parameters indicate types of Nodes and Edges.
 * <E> must be a class with default constructor and virtual destructor. Edges inherite from <E>.
 * <N> must be a class with default constructor and virtual destructor. Nodes inherite from <N>.
 *
 * Graph deletion deletes all nodes and edges.
 *
 * DirectedGraph different from UndirectedGraph in createNode() method.
 * First create DirectedNode, second - UndirectedNode.
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <assert.h>

template<typename N, typename E>
class Graph;
template<typename N, typename E>
class DirectedGraph;
template<typename N, typename E>
class UndirectedGraph;

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
        for (auto& pair : _nodes)
            delete pair.second;
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
    Graph() = default;
    Graph(const Graph<N, E> &graph) = delete;
    Graph<N, E> &operator=(const Graph<N, E> &graph) = delete;

    std::map<int, Node<N, E>*>    _nodes;
    std::vector<Edge<N, E>*>      _edges;
};

template<typename N, typename E>
class DirectedGraph : public Graph<N, E>
{
public:
    DirectedGraph() = default;
    DirectedGraph(const DirectedGraph<N, E> &graph) = delete;
    DirectedGraph<N, E> &operator=(const DirectedGraph<N, E> &graph) = delete;
    virtual ~DirectedGraph() override = default;

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
    UndirectedGraph() = default;
    UndirectedGraph(const UndirectedGraph<N, E> &graph) = delete;
    UndirectedGraph<N, E> &operator=(const UndirectedGraph<N, E> &graph) = delete;
    virtual ~UndirectedGraph() override = default;

    virtual Node<N, E> *createNode() override {
        int id = this->_nodes.empty() ? 1 : this->_nodes.rbegin()->first + 1;
        auto node = new UndirectedNode<N, E>(this, id);
        auto pair = this->_nodes.insert(std::make_pair(id, node));
        assert(pair.second == true);
        return node;
    }
};

#endif // GRAPH_H
