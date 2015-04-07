#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>

#include "graph.h"

class QGraphicsNode;
class QGraphicsEdge;

struct NodeInfo;
struct EdgeInfo;

class GraphScene : public QGraphicsScene
{
public:
    typedef Graph<NodeInfo, EdgeInfo> graph_type;
    typedef Node<NodeInfo, EdgeInfo> node_type;
    typedef Edge<NodeInfo, EdgeInfo> edge_type;

    GraphScene(graph_type *graph, QObject *parent = 0);
    virtual ~GraphScene();

protected:
    virtual void    mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void	mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void	mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void	mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void    keyPressEvent(QKeyEvent *keyEvent);

private:
    graph_type *_graph;
};

class DirectedGraphScene : public GraphScene
{
public:
    DirectedGraphScene(QObject *parent = 0);
    virtual ~DirectedGraphScene();
};

class UndirectedGraphScene : public GraphScene
{
public:
    UndirectedGraphScene(QObject *parent = 0);
    virtual ~UndirectedGraphScene();
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
