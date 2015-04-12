#ifndef QGRAPHICSEDGE_H
#define QGRAPHICSEDGE_H

#include <QObject>
#include <QGraphicsPathItem>
#include <QPen>

#include "item_info.h"

class WeightEdgeTextItem;

class QGraphicsEdge : public QObject, public QGraphicsPathItem
{
    Q_OBJECT

public:
    virtual ~QGraphicsEdge() override;

    virtual int type() const override = 0;
    virtual void draw(QGraphicsNode *fromGNode, QGraphicsNode *toGNode) = 0;
    virtual void draw(QGraphicsNode *fromGNode, const QPointF &toPoint) = 0;

    void setFont(const QFont &font, const QColor &color = Qt::black);
    QFont font() const;

    virtual void setBrush(const QBrush &brush);
    virtual QBrush brush() const;

    virtual void setActivePen(const QPen &pen);

    void setPen(const QPen &pen);
    const QPen &pen() const;

    void setHoverPen(const QPen &pen);
    const QPen &hoverPen() const;

    void setPenTransparency(int minWeight, int maxWeight);
    void join(QGraphicsNode *fromGNode, QGraphicsNode *toGNode);
    void refresh();
    void showWeight();

    WeightedEdge *edge() const;

    static QPointF calcIntermediatePoint(const QPointF &pointFrom, const QPointF &pointTo, qreal radius);

signals:
    void created(QGraphicsEdge *gEdge);
    void changed(int from, QGraphicsEdge *gEdge);
    void removed(int weight);

public slots:
    void deleteCompletely();

protected:
    QGraphicsEdge(WeightedEdge *edge, QGraphicsItem *parent = 0);

    virtual void    hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void    hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void    focusInEvent(QFocusEvent *event) override;

    WeightedEdge        *_edge;
    WeightEdgeTextItem  *_weightItem;

    QPen    _simplePen;
    QPen    _hoverPen;

private slots:
    void setWeight(int weight);
};

#endif // QGRAPHICSEDGE_H
