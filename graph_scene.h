#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>

#include "graph.h"

class QGraphicsNode;
class QGraphicsEdge;

struct NodeInfo;
struct EdgeInfo;

class BasicGraphScene : public QGraphicsScene
{
public:
    typedef Graph<NodeInfo, EdgeInfo> graph_type;
    typedef Node<NodeInfo, EdgeInfo> node_type;
    typedef Edge<NodeInfo, EdgeInfo> edge_type;

    virtual ~BasicGraphScene();

    virtual QGraphicsNode *createGraphicsNode(node_type *node) = 0;
    virtual QGraphicsEdge *createGraphicsEdge(edge_type *edge) = 0;

    void addNode(const QPointF &centerPos, int weight);
    void addEdge(int weight);

protected:
    BasicGraphScene(graph_type *graph, QObject *parent = 0);

    virtual void    mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void	mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void	mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void	mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void    keyPressEvent(QKeyEvent *keyEvent);

private:
    graph_type *_graph;
};

template<typename GN, typename GE>
class GraphScene : public BasicGraphScene
{
public:
    virtual ~GraphScene() {}

    virtual QGraphicsNode *createGraphicsNode(node_type *node) override final {
        return new GN(node);
    }
    virtual QGraphicsEdge *createGraphicsEdge(edge_type *edge) override final {
        return new GE(node);
    }

protected:
    GraphScene(graph_type *graph, QObject *parent = 0) : BasicGraphScene(graph, parent) {
        static_assert(std::is_base_of<QGraphicsNode, GN>::value, "GN type must be derived from QGraphicsNode");
        static_assert(std::is_base_of<QGraphicsEdge, GE>::value, "GE type must be derived from QGraphicsEdge");
    }
};

template<typename GN, typename GE>
class DirectedGraphScene : public GraphScene<GN, GE>
{
public:
    DirectedGraphScene(QObject *parent = 0) : GraphScene<GN, GE>(new DirectedGraph<NodeInfo, EdgeInfo>(), parent) {}
    virtual ~DirectedGraphScene() {}
};

template<typename GN, typename GE>
class UndirectedGraphScene : public GraphScene<GN, GE>
{
public:
    UndirectedGraphScene(QObject *parent = 0) : GraphScene<GN, GE>(new UndirectedGraph<NodeInfo, EdgeInfo>(), parent) {}
    virtual ~UndirectedGraphScene() {}
};


struct NodeInfo {
public:
    NodeInfo() : _weight(1), _graphicsNode(nullptr), _pos(QPointF()) {}
    virtual ~NodeInfo() {}

    void setWeight(int weight) { _weight = weight; }
    void setGraphicsNode(QGraphicsNode *gn) {_graphicsNode = gn; }
    void setPos(const QPointF &pos) { _pos = pos; }

    int weight() const { return _weight; }
    QGraphicsNode *graphicsNode() const { return _graphicsNode; }
    QPointF pos() const { return _pos; }

private:
    int             _weight;
    QGraphicsNode  *_graphicsNode;
    QPointF         _pos;
};

struct EdgeInfo {
public:
    EdgeInfo() : _weight(1), _graphicsEdge(nullptr) {}
    virtual ~EdgeInfo() {}

    void setWeight(int weight) { _weight = weight; }
    void setGraphicsNode(QGraphicsEdge *ge) { _graphicsEdge = ge; }

    int weight() const { return _weight; }
    QGraphicsEdge *graphicsEdge() const { return _graphicsEdge; }

private:
    int             _weight;
    QGraphicsEdge  *_graphicsEdge;
};

#endif // GRAPHSCENE_H
