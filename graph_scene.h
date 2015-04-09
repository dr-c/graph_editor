#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>

#include "item_info.h"

class QGraphicsNode;
class QGraphicsEdge;

class GraphSceneMode;

class BasicGraphScene : public QGraphicsScene
{
public:
    virtual ~BasicGraphScene();

    virtual QGraphicsNode *createGraphicsNode(WeightedNode *node) = 0;
    virtual QGraphicsEdge *createGraphicsEdge(WeightedEdge *edge) = 0;
    virtual int typeGraphicsNode() const = 0;
    virtual int typeGraphicsEdge() const = 0;

    void addNode(const QPointF &centerPos, int weight = 1);
    void addEdge(int weight = 1);

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
    GraphScene(WeightedGraph *graph, GraphSceneMode *mode, QObject *parent = 0) : BasicGraphScene(graph, mode, parent) {
        static_assert(std::is_base_of<QGraphicsNode, GN>::value, "GN type must be derived from QGraphicsNode");
        static_assert(std::is_base_of<QGraphicsEdge, GE>::value, "GE type must be derived from QGraphicsEdge");
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
