/*
 * QGraphicsEdge - graphical representation of WeightedEdges.
 *
 * QGraphicsEdge is abstract, so need to use derived classes.
 *
 * Methods draw() are called every time, position one of ends is changed.
 * Use it only before joining, otherwise use refresh().
 * join() performs connection of two Nodes.
 *
 * QGraphicsEdge created with hidden WeightItem. To display it, use showWeight().
 *
 * Trancparency of Edges depends on weight. The higher weight, the larger opaque value.
 *
 * calcIntermediatePoint(A, B, R) function calculates intermediate point,
 *  on line, between points A and B, at distance R from point B.
 *
 * < !!! CAREFUL !!!
 * There are two ways to delete QGraphicsEdge:
 *  - Destructor. In this case only QGraphicsEdge(responsible for graphical representation)
 *     will be deleted. Edge(logical representation) will not be deleted.
 *  - Method deleteCompletely(). In this case will be deleted both
 *     Edge and next immediately QGraphicsEdge.
 * !!! CAREFUL !!! >
 *
 * QGraphicsEdge emits three types of signals, to notify Scene
 *  about possible necessary to recalculate transparency:
 *  - created(gEdge*) after join().
 *  - changed(int, gEdge*) after changing the weight.
 *  - removed(int) deletion only via deleteCompletely().
 */

#ifndef QGRAPHICSEDGE_H
#define QGRAPHICSEDGE_H

#include <QObject>
#include <QGraphicsPathItem>
#include <QPen>

#include "item_info.h"

class WeightEdgeTextItem;
class GraphScene;

class QGraphicsEdge : public QObject, public QGraphicsPathItem
{
    Q_OBJECT

public:
    virtual ~QGraphicsEdge() override;
    void deleteCompletely();

    virtual int type() const override = 0;
    virtual void draw(QGraphicsNode *fromGNode, QGraphicsNode *toGNode) = 0;
    virtual void draw(QGraphicsNode *fromGNode, const QPointF &toPoint) = 0;

    void setFont(const QFont &font, const QColor &color = Qt::black);
    QFont font() const;

    virtual void setBrush(const QBrush &brush);
    virtual QBrush brush() const;

    void setPen(const QPen &pen);
    const QPen &pen() const;

    void setHoverPen(const QPen &pen);
    const QPen &hoverPen() const;

    void setPenTransparency(int minWeight, int maxWeight);
    void join(QGraphicsNode *fromGNode, QGraphicsNode *toGNode);
    void refresh();
    void showWeight();

    WeightedEdge *edge() const;

    void changeWeightOutside(int weight);

    static QPointF calcIntermediatePoint(const QPointF &pointFrom, const QPointF &pointTo, qreal radius);

signals:
    void created(QGraphicsEdge *gEdge);
    void changed(int from, QGraphicsEdge *gEdge);
    void removed(int weight);

protected:
    QGraphicsEdge(GraphScene *scene, WeightedEdge *edge, QGraphicsItem *parent = 0);

    virtual void    setActivePen(const QPen &pen);

    virtual void    hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void    hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void    focusInEvent(QFocusEvent *event) override;

    GraphScene     *_scene;
    WeightedEdge        *_edge;
    WeightEdgeTextItem  *_weightItem;

    QPen    _simplePen;
    QPen    _hoverPen;

    int _beforeWeightChanging;

protected slots:
    void prepareDeletion();
    void startWeightFixed(int weight);
    void weightFixed(int weight);

private slots:
    void setWeight(int weight);
};

#endif // QGRAPHICSEDGE_H
