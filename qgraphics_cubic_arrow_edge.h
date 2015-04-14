/*
 * QGraphicsCubicArrowEdge - curve(based on cubic Bezier) with triangle at the end(arrow).
 *
 * QGraphicsCubicArrowEdge one of the implementation of QGraphicsEdge.
 * It is best to use QGraphicsCubicArrowEdge with DirectedGraph, as DirectedEdge.
 */

#ifndef QGRAPHICSCUBICARROWEDGE_H
#define QGRAPHICSCUBICARROWEDGE_H

#include "qgraphics_edge.h"

class QGraphicsCubicArrowEdge : public QGraphicsEdge
{
public:
    enum { Type = UserType + 202 };

    QGraphicsCubicArrowEdge(BasicGraphScene *scene, WeightedEdge *edge, QGraphicsItem *parent = 0);
    virtual ~QGraphicsCubicArrowEdge() override;

    virtual int type() const override;
    virtual void draw(QGraphicsNode *fromGNode, QGraphicsNode *toGNode) override;
    virtual void draw(QGraphicsNode *fromGNode, const QPointF &toPoint) override;

protected:
    virtual void setActivePen(const QPen &pen) override;

private:
    static QPolygonF calcTrianglePolygon(const QPointF &pointFrom, const QPointF &pointTo, const qreal height, const qreal width);
    static QPointF calcCubicPoint(const QPointF &pointFrom, const QPointF &pointTo, const qreal len_persentage, const qreal height);

    class CurvePathItem : public QGraphicsPathItem
    {
    public:
        enum { Type = UserType + 203 };

        CurvePathItem(QGraphicsCubicArrowEdge *parent = 0)
            : QGraphicsPathItem(parent), _parent(parent) {
            setFlags(QGraphicsItem::ItemIsFocusable);
            setAcceptHoverEvents(true);
        }
        virtual ~CurvePathItem() override = default;

        virtual int type() const override { return Type; }

    protected:
        virtual void    hoverEnterEvent(QGraphicsSceneHoverEvent *event) override {
            QGraphicsPathItem::hoverEnterEvent(event);
            _parent->hoverEnterEvent(event);
        }
        virtual void    hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override {
            QGraphicsPathItem::hoverLeaveEvent(event);
            _parent->hoverLeaveEvent(event);
        }
        virtual void    focusInEvent(QFocusEvent *event) override {
            QGraphicsPathItem::focusInEvent(event);
            _parent->setFocus();
        }

    private:
        QGraphicsCubicArrowEdge *_parent;
    };

    CurvePathItem *_curve;
};

#endif // QGRAPHICSCUBICARROWEDGE_H
