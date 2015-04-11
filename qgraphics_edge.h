#ifndef QGRAPHICSEDGE_H
#define QGRAPHICSEDGE_H

#include <QObject>
#include <QGraphicsPathItem>

#include "item_info.h"

class WeightEdgeTextItem;

class QGraphicsEdge : public QObject, public QGraphicsPathItem
{
    Q_OBJECT

public:
    virtual ~QGraphicsEdge() override;
    void deleteCompletely();

    virtual int type() const = 0;
    virtual void draw(QGraphicsNode *fromNode, QGraphicsNode *toNode) = 0;
    virtual void draw(QGraphicsNode *fromNode, const QPointF &toPoint) = 0;

    void setFont(const QFont &font, const QColor &color = Qt::black);
    void setBrush(const QBrush &brush);

    QFont font() const;
    QBrush brush() const;

    void join(QGraphicsNode *fromNode, QGraphicsNode *toNode);
    void refresh();
    void showWeight();

    WeightedEdge *edge() const;

protected:
    QGraphicsEdge(WeightedEdge *edge, QGraphicsItem *parent = 0);

    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;

    WeightedEdge        *_edge;
    WeightEdgeTextItem  *_weightItem;

private slots:
    void setWeight(int weight);
};

#endif // QGRAPHICSEDGE_H
