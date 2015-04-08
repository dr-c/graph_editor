#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>

#include "item_info.h"

class QGraphicsNode;
class QGraphicsEdge;

struct NodeInfo;
struct EdgeInfo;

class BasicGraphScene : public QGraphicsScene
{
public:
    virtual ~BasicGraphScene();

    virtual QGraphicsNode *createGraphicsNode(WeightedNode *node) = 0;
    virtual QGraphicsEdge *createGraphicsEdge(WeightedEdge *edge) = 0;
    virtual int typeGraphicsNode() const = 0;
    virtual int typeGraphicsEdge() const = 0;

    void addNode(const QPointF &centerPos, int weight);
    void addEdge(int weight);

protected:
    BasicGraphScene(WeightedGraph *graph, QObject *parent = 0);

    virtual void    mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void	mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void	mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void	mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void    keyPressEvent(QKeyEvent *keyEvent);

private:
    WeightedGraph *_graph;
};

template<typename GN, typename GE>
class GraphScene : public BasicGraphScene
{
public:
    virtual ~GraphScene() {}

    virtual QGraphicsNode *createGraphicsNode(WeightedNode *node) override final {
        return new GN(node);
    }
    virtual QGraphicsEdge *createGraphicsEdge(WeightedEdge *edge) override final {
        return new GE(edge);
    }
    virtual int typeGraphicsNode() const override final {
        return GN::Type;
    }
    virtual int typeGraphicsEdge() const override final {
        return GE::Type;
    }

protected:
    GraphScene(WeightedGraph *graph, QObject *parent = 0) : BasicGraphScene(graph, parent) {
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

#endif // GRAPHSCENE_H
