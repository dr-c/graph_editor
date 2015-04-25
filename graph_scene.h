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
#include "graph_configuration.h"
#include "qgraphics_edge.h"
#include "qgraphics_node.h"

class GraphSceneMode;

class GraphScene : public QGraphicsScene
{
    Q_OBJECT

public:
    GraphScene(std::shared_ptr<WeightedGraph> &&graph, std::shared_ptr<GraphConfiguration> &&config, GraphSceneMode *mode, QObject *parent = 0);
    virtual ~GraphScene();

    QGraphicsNode *addNode(const QPointF &centerPos, int weight = 1, int id = 0);
    QGraphicsNode *addNode(WeightedNode *node);
    QGraphicsEdge *addEdge(int weight = 1);
    QGraphicsEdge *addEdge(WeightedEdge *edge);

    History *history() const;
    const std::shared_ptr<WeightedGraph> &graph() const;
    const GraphSceneMode *mode() const;
    void setMode(GraphSceneMode *mode);
    const std::shared_ptr<GraphConfiguration> &config();
    void setConfig(std::shared_ptr<GraphConfiguration> &&config);

protected:
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
    std::shared_ptr<WeightedGraph> _graph;
    std::shared_ptr<GraphConfiguration> _config;
    GraphSceneMode  *_mode;

    void setGNodeDesign(QGraphicsNode *gNode);
    void setGEdgeDesign(QGraphicsEdge *gEdge);

    int _minEdgeWeight;
    int _maxEdgeWeight;
};

#endif // GRAPHSCENE_H
