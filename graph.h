/*
 * Graph - logical representation of a set of objects(nodes),
 *  where some objects are connects by links (edges).
 *
 * Graph stores two sets: Nodes and Edges, which were created by createNode()/createEdge() methods.
 * This sets can be accessible via nodes() and edges() respectively.
 *
 * Graph is abstract factory for Nodes and Edges.
 * It provides functionality not only for creating(createNode()/createEdge()),
 *  but also and deleting(remove(Node*)/remove(Edge*)) Nodes and Edges.
 * Deleting may be accomplished from Node/Edge directly via remove(). It`s actualy the same.
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
        for (auto& pair : _nodes)
            delete pair.second;
    }

    Node<N, E> *createNode(int id = 0) {
        if (id == 0)
            id = this->_nodes.empty() ? 1 : this->_nodes.rbegin()->first + 1;
        assert(this->_nodes.find(id) == this->_nodes.end());
        Node<N, E> *node = constructNode(id);
        auto pair = this->_nodes.insert(std::make_pair(id, node));
        assert(pair.second == true);
        return node;
    }

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
        auto iter = std::find(_edges.begin(), _edges.end(), edge);
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

    virtual bool isDirected() const = 0;

protected:
    Graph() = default;
    Graph(const Graph<N, E> &graph) = delete;
    Graph<N, E> &operator=(const Graph<N, E> &graph) = delete;

    virtual Node<N, E> *constructNode(int id) = 0;

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
    virtual bool isDirected() const { return true; }

protected:
    virtual Node<N, E> *constructNode(int id) override {
        return new DirectedNode<N, E>(this, id);
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
    virtual bool isDirected() const { return false; }

protected:
    virtual Node<N, E> *constructNode(int id) override {
        return new UndirectedNode<N, E>(this, id);
    }
};

#endif // GRAPH_H
