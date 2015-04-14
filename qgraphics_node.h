/*
 * QGraphicsNode - graphical representation of WeightedNodes.
 *
 * QGraphicsNode is abstract, so need to use derived classes.
 *
 * Methods shape(), boundingRect() and paint() inherited from QGraphicsItem.
 * Method calcIntermediatePoint(point) calculate intersection point
 *  between line from center of Node to point and Node periphery.
 * Method intersects(gNode*) checks if two QGraphicsNodes of the same type intersects.
 * Method setGeometry(QPointF) used for the correct placement of subItems.
 * All of those methods must be reimplemented in derived classes.
 *
 * The size of gNode depends of weight. The higher weight, the larger gNode size.
 * The dependence is nonlinear. size = sqrt(weight+k1)*k2,
 *  where k1 and k2 constant coefficients.
 *
 * < !!! CAREFUL !!!
 * There are two ways to delete QGraphicsNode:
 *  - Destructor. In this case only QGraphicsNode(responsible for graphical representation)
 *     will be deleted. Node(logical representation) will not be deleted.
 *  - Method deleteCompletely(). In this case will be deleted both
 *     Node and next immediately QGraphicsNode with adjoined QGraphicsEdges.
 * !!! CAREFUL !!! >
 *
 * Changing in QGraphicsNode cause changing in all adjoined QGraphicsEdges.
 */

#ifndef QGRAPHICSNODE_H
#define QGRAPHICSNODE_H

#include <QGraphicsObject>
#include <QPen>

#include "item_info.h"

class BasicGraphScene;
class WeightTextItem;

class QGraphicsNode : public QGraphicsObject
{
    Q_OBJECT
public:
    virtual ~QGraphicsNode() override;
    virtual void deleteCompletely();

    virtual int type() const override = 0;
    virtual QPainterPath shape() const override = 0;
    virtual QRectF boundingRect() const override = 0;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override = 0;

    virtual QPointF calcIntermediatePoint(const QPointF &toPoint) const = 0;

    virtual bool intersects(QGraphicsNode *gNode) const = 0;

    virtual void setActivePen(const QPen &pen) = 0;

    virtual void setFont(const QFont &font, const QColor &color = Qt::black) = 0;
    virtual QFont font() const = 0;

    virtual void setBrush(const QBrush &brush) = 0;
    virtual QBrush brush() const = 0;

    void setPen(const QPen &pen);
    const QPen &pen() const;

    void setHoverPen(const QPen &pen);
    const QPen &hoverPen() const;

    qreal radius() const;
    WeightedNode *node() const;

    void changePositionOutside(const QPointF &topLeft);
    void changeWeightOutside(int weight);

protected slots:
    void setWeight(int weight);
    void weightFixed(int weight);

protected:
    QGraphicsNode(BasicGraphScene *scene, WeightedNode *node, QGraphicsItem *parent = 0);

    virtual void        setGeometry(const QPointF &centerPos) = 0;
    virtual void        calcRadius(int weight);

    virtual void        mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    virtual void        mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    virtual void        mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void        keyPressEvent(QKeyEvent *event) override;
    virtual QVariant    itemChange(GraphicsItemChange change, const QVariant &value) override;
    virtual void        hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void        hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    BasicGraphScene  *_scene;
    WeightedNode *_node;

    WeightTextItem          *_weightItem;
    QGraphicsSimpleTextItem *_idItem;

    qreal   _radius;

    QPen    _simplePen;
    QPen    _hoverPen;

    int     _beforeWeightChanging;
    QPointF _beforeMousePressPos;

private:
    void updateRelatedEdges();

    static  const       int     RADIUS_SHIFT = 25;
    static  constexpr   qreal   RADIUS_SCALE = 5.;
};

#endif // QGRAPHICSNODE_H
