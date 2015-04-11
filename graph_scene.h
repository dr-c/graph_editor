#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>

#include "item_info.h"
#include "qgraphics_edge.h"
#include "qgraphics_node.h"

class GraphSceneMode;

class BasicGraphScene : public QGraphicsScene
{
    Q_OBJECT

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

    void setNodePen(const QPen &pen)        { _nodePen = pen;       }
    void setnodeHoverPen(const QPen &pen)   { _nodeHoverPen = pen;  }
    void setedgePen(const QPen &pen)        { _edgePen = pen;       }
    void setedgeHoverPen(const QPen &pen)   { _edgeHoverPen = pen;  }
    void setItemFont(const QFont &font)     { _itemFont = font;     }
    void setNodeBrush(const QBrush &brush)  { _nodeBrush = brush;   }
    void setedgeBrush(const QBrush &brush)  { _edgeBrush = brush;   }

    const QPen &nodePen() const             { return _nodePen;      }
    const QPen &nodeHoverPen() const        { return _nodeHoverPen; }
    const QPen &edgePen() const             { return _edgePen;      }
    const QPen &edgeHoverPen() const        { return _edgeHoverPen; }
    const QFont &itemFont() const           { return _itemFont;     }
    const QBrush &nodeBrush() const         { return _nodeBrush;    }
    const QBrush &edgeBrush() const         { return _edgeBrush;    }

protected:
    BasicGraphScene(WeightedGraph *graph, GraphSceneMode *mode, QObject *parent = 0);

    virtual QGraphicsNode *createGraphicsNode(WeightedNode *node) const = 0;
    virtual QGraphicsEdge *createGraphicsEdge(WeightedEdge *edge) const = 0;

    virtual void    mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    virtual void	mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    virtual void	mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    virtual void	mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    virtual void    keyPressEvent(QKeyEvent *keyEvent) override;
    virtual void    keyReleaseEvent(QKeyEvent *keyEvent) override;

private slots:
    void calcEdgesTransparencyOnCreate(QGraphicsEdge *gEdge);
    void calcEdgesTransparencyOnChange(int fromWeight, QGraphicsEdge *gEdge);
    void calcEdgesTransparencyOnDelete(int weight);

private:
    bool calcEdgesWeightRange();

    WeightedGraph   *_graph;
    GraphSceneMode  *_mode;

    int _minEdgeWeight;
    int _maxEdgeWeight;

    QPen    _nodePen;
    QPen    _nodeHoverPen;
    QPen    _edgePen;
    QPen    _edgeHoverPen;
    QFont   _itemFont;
    QBrush  _nodeBrush;
    QBrush  _edgeBrush;
};

template<typename GN, typename GE>
class GraphScene : public BasicGraphScene
{
public:
    virtual ~GraphScene() = default;

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

    virtual QGraphicsNode *createGraphicsNode(WeightedNode *node) const override final {
        return new GN(node);
    }
    virtual QGraphicsEdge *createGraphicsEdge(WeightedEdge *edge) const override final {
        return new GE(edge);
    }
};

template<typename GN, typename GE>
class DirectedGraphScene : public GraphScene<GN, GE>
{
public:
    DirectedGraphScene(DirectedGraph<NodeInfo, EdgeInfo> *graph, GraphSceneMode *mode, QObject *parent = 0)
        : GraphScene<GN, GE>(graph, mode, parent) {}
    virtual ~DirectedGraphScene() = default;
};

template<typename GN, typename GE>
class UndirectedGraphScene : public GraphScene<GN, GE>
{
public:
    UndirectedGraphScene(UndirectedGraph<NodeInfo, EdgeInfo> *graph, GraphSceneMode *mode, QObject *parent = 0)
        : GraphScene<GN, GE>(graph, mode, parent) {}
    UndirectedGraphScene(const UndirectedGraphScene<GN, GE> &scene) = delete;
    UndirectedGraphScene<GN, GE> &operator=(const UndirectedGraphScene<GN, GE> &scene) = delete;
    virtual ~UndirectedGraphScene() = default;
};

#endif // GRAPHSCENE_H
