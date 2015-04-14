/*
 * BasicGraphScene - graphical representation of Graph.
 *
 * BasicGraphScene operates only with Graph<NodeInfo, EdgeInfo>(WeightedGraph).
 * For operate with another type of Graphs, use another type of Scene.
 *
 * The class serves as a container for QGraphicsNodes and QGraphicsEdges.
 * It is used together with GraphView for visualizng Graph.
 * It also can be used with default QGraphicsView.
 *
 * BasicGraphScene can work in 2 modes(Scaleable. Can create more).
 * Mode determines behavior for QGraphicsNode and presents interaction for Scene.
 * For detail see GraphSceneMode description.
 * For changing mode use setMode(Mode*).
 * It`s not recommended to set Mode which already active (inefficiently).
 * To avoid that, compare established mode(via mode() method) with others.
 *
 * Methods addNode(QPointF,int) and addEdge(int) create new Node/Edge in
 *  logical Graph, create new QGraphicsNode/QGraphicsEdge and add it to Scene.
 * Methods addNode(Node*)/addEdge(Edge*) create only QGraphicsNode/QGraphicsEdge
 *  from already exist Node/Edge and add it to scene.
 *
 * Constructor takes Graph and mode. If Graph contains some nodes/edges
 *  they will be displayed on Scene.
 * For creating Scene use DirectedGraphScene or UndirectedGraphScene
 *  constructors, depending on Graph type(Directed/Undirected).
 * Actually, now there is no difference between them, but later functionality
 *  of these classes might be expanded.
 *
 * <GN> and <GE> templates parameters on this classes determine type of
 *  QGraphicsItem which will be created on addNode(Node*)/addEdge(Edge*) method call.
 * <GN> must be a class derived from QGraphicsNode.
 * <GE> must be a class derived from QGraphicsEdge.
 *
 * < !!! CAREFUL !!!
 * Destructor doesn`t delete Graph and contained Nodes with Edges.
 * It`s necessary to save pointer to Graph on creating Scene, or get it
 *  before destroying the Scene via graph() method and delete it later.
 * Possible to use this Graph with other Scene, after deleting this Scene.
 * !!! CAREFUL !!! >
 */

#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>

#include "history.h"
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

    History *history() const;
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

    virtual QGraphicsNode *createGraphicsNode(WeightedNode *node) = 0;
    virtual QGraphicsEdge *createGraphicsEdge(WeightedEdge *edge) = 0;

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
    bool calcEdgesWeightRange();

private:

    History         *_history;
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

    virtual QGraphicsNode *createGraphicsNode(WeightedNode *node) override final {
        return new GN(this, node);
    }
    virtual QGraphicsEdge *createGraphicsEdge(WeightedEdge *edge) override final {
        return new GE(this, edge);
    }
};

template<typename GN, typename GE>
class DirectedGraphScene : public GraphScene<GN, GE>
{
public:
    DirectedGraphScene(DirectedGraph<NodeInfo, EdgeInfo> *graph, GraphSceneMode *mode, QObject *parent = 0)
        : GraphScene<GN, GE>(graph, mode, parent) {}
    DirectedGraphScene(const DirectedGraphScene<GN, GE> &scene) = delete;
    DirectedGraphScene<GN, GE> &operator=(const DirectedGraphScene<GN, GE> &scene) = delete;
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
