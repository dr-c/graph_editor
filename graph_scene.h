#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>

#include "item_info.h"
#include "qgraphics_edge.h"
#include "qgraphics_node.h"

class GraphSceneMode;

class BasicGraphScene : public QGraphicsScene
{
public:
    virtual ~BasicGraphScene();

    virtual int typeGraphicsNode() const = 0;
    virtual int typeGraphicsEdge() const = 0;

    QGraphicsNode *addNode(const QPointF &centerPos, int weight = 1);
    QGraphicsNode *addNode(WeightedNode *node);
    QGraphicsEdge *addEdge(int weight = 1);
    QGraphicsEdge *addEdge(WeightedEdge *edge);

    const WeightedGraph *graph() const;
    const GraphSceneMode *mode() const;
    void setMode(GraphSceneMode *mode);

    void setNodePen(const QPen &pen);
    void setNodeFont(const QFont &font);
    void setNodeBrush(const QBrush &brush);

    QPen nodePen() const;
    QFont nodeFont() const;
    QBrush nodeBrush() const;

protected:
    BasicGraphScene(WeightedGraph *graph, GraphSceneMode *mode, QObject *parent = 0);

    virtual QGraphicsNode *createGraphicsNode(WeightedNode *node) = 0;
    virtual QGraphicsEdge *createGraphicsEdge(WeightedEdge *edge) = 0;

    virtual void    mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void	mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void	mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void	mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void    keyPressEvent(QKeyEvent *keyEvent);
    virtual void    keyReleaseEvent(QKeyEvent *keyEvent);

private:
    WeightedGraph   *_graph;
    GraphSceneMode  *_mode;

    QPen    _nodePen;
    QFont   _nodeFont;
    QBrush  _nodeBrush;
};

template<typename GN, typename GE>
class GraphScene : public BasicGraphScene
{
public:
    virtual ~GraphScene() {}

    virtual int typeGraphicsNode() const override final {
        return GN::Type;
    }
    virtual int typeGraphicsEdge() const override final {
        return GE::Type;
    }

protected:
    GraphScene(WeightedGraph *graph, GraphSceneMode *mode, QObject *parent = 0) : BasicGraphScene(graph, mode, parent) {
        static_assert(std::is_base_of<QGraphicsNode, GN>::value, "GN type must be derived from QGraphicsNode");
        static_assert(std::is_base_of<QGraphicsEdge, GE>::value, "GE type must be derived from QGraphicsEdge");

        for (auto pair : graph->nodes()) {
            assert(pair.second->graphicsNode() == nullptr);
            addNode(pair.second);
        }
        for (auto edge : graph->edges()) {
            assert(edge->graphicsEdge() == nullptr);
            addEdge(edge)->showWeight();
        }
    }

    virtual QGraphicsNode *createGraphicsNode(WeightedNode *node) override final {
        return new GN(node);
    }
    virtual QGraphicsEdge *createGraphicsEdge(WeightedEdge *edge) override final {
        return new GE(edge);
    }
};

template<typename GN, typename GE>
class DirectedGraphScene : public GraphScene<GN, GE>
{
public:
    DirectedGraphScene(DirectedGraph<NodeInfo, EdgeInfo> *graph, GraphSceneMode *mode, QObject *parent = 0) : GraphScene<GN, GE>(graph, mode, parent) {}
    virtual ~DirectedGraphScene() {}
};

template<typename GN, typename GE>
class UndirectedGraphScene : public GraphScene<GN, GE>
{
public:
    UndirectedGraphScene(UndirectedGraph<NodeInfo, EdgeInfo> *graph, GraphSceneMode *mode, QObject *parent = 0) : GraphScene<GN, GE>(graph, mode, parent) {}
    virtual ~UndirectedGraphScene() {}
};

#endif // GRAPHSCENE_H
