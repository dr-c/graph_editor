/*
 * NodeInfo and EdgeInfo classes which used as template parameters for Graph, Edge and Node.
 * WeightedGraph(Graph<NodeInfo, EdgeInfo>) widely used by BasicGraphScene,
 *  QGraphicsNodes and QGraphicsEdges.
 * For different purposes you can create and use another classes.
 */

#ifndef ITEM_INFO_H
#define ITEM_INFO_H

class QGraphicsNode;
class QGraphicsEdge;

#include <QPointF>
#include "graph.h"

struct NodeInfo {
public:
    NodeInfo() : _weight(1), _graphicsNode(nullptr), _pos(QPointF()) {}
    virtual ~NodeInfo() = default;

    void setWeight(int weight)              { _weight       = weight;   }
    void setGraphicsNode(QGraphicsNode *gn) { _graphicsNode = gn;       }
    void setPos(const QPointF &pos)         { _pos          = pos;      }

    int weight() const                      { return _weight;           }
    QGraphicsNode *graphicsNode() const     { return _graphicsNode;     }
    const QPointF &pos() const              { return _pos;              }

private:
    int             _weight;
    QGraphicsNode  *_graphicsNode;
    QPointF         _pos;
};

struct EdgeInfo {
public:
    EdgeInfo() : _weight(1), _graphicsEdge(nullptr) {}
    virtual ~EdgeInfo() = default;

    void setWeight(int weight)              { _weight       = weight;   }
    void setGraphicsEdge(QGraphicsEdge *ge) { _graphicsEdge = ge;       }

    int weight() const                      { return _weight;           }
    QGraphicsEdge *graphicsEdge() const     { return _graphicsEdge;     }

private:
    int             _weight;
    QGraphicsEdge  *_graphicsEdge;
};

typedef Graph<NodeInfo, EdgeInfo>   WeightedGraph;
typedef Node<NodeInfo, EdgeInfo>    WeightedNode;
typedef Edge<NodeInfo, EdgeInfo>    WeightedEdge;

#endif // ITEM_INFO_H

