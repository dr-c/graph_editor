#ifndef GRAPHCONFIGURATION_H
#define GRAPHCONFIGURATION_H

#include "qgraphics_node.h"
#include "qgraphics_edge.h"
#include <memory>

class GraphicsNodeCreator {
public:
    virtual ~GraphicsNodeCreator() = default;
    virtual int type() const = 0;
    virtual QGraphicsNode* create(GraphScene *scene, WeightedNode *node) const = 0;
protected:
    GraphicsNodeCreator() = default;
};
template<typename T>
class GraphicsNodeCreatorT : public GraphicsNodeCreator {
public:
    GraphicsNodeCreatorT() {
        static_assert(std::is_base_of<QGraphicsNode, T>::value, "T type must be derived from QGraphicsNode");
    }
    virtual ~GraphicsNodeCreatorT() = default;
    virtual int type() const { return T::Type; }
    virtual QGraphicsNode* create(GraphScene *scene, WeightedNode *node) const {
        return new T(scene, node);
    }
};

class GraphicsEdgeCreator {
public:
    virtual ~GraphicsEdgeCreator() = default;
    virtual int type() const = 0;
    virtual QGraphicsEdge* create(GraphScene *scene, WeightedEdge *edge) const = 0;
protected:
    GraphicsEdgeCreator() = default;
};
template<typename T>
class GraphicsEdgeCreatorT : public GraphicsEdgeCreator {
public:
    GraphicsEdgeCreatorT() {
        static_assert(std::is_base_of<QGraphicsEdge, T>::value, "T type must be derived from QGraphicsEdge");
    }
    virtual ~GraphicsEdgeCreatorT() = default;
    virtual int type() const { return T::Type; }
    virtual QGraphicsEdge* create(GraphScene *scene, WeightedEdge *edge) const {
        return new T(scene, edge);
    }
};


struct GraphConfiguration
{
    std::unique_ptr<GraphicsNodeCreator> _nodeCreator;
    std::unique_ptr<GraphicsEdgeCreator> _edgeCreator;
    QPen _nodePen;
    QPen _nodeHoverPen;
    QBrush _nodeBrush;
    QFont _nodeFont;
    QPen _edgePen;
    QPen _edgeHoverPen;
    QBrush _edgeBrush;
    QFont _edgeFont;
};

#endif // GRAPHCONFIGURATION_H
