#ifndef QGRAPHICSELLIPSENODE_H
#define QGRAPHICSELLIPSENODE_H

#include "qgraphics_node.h"

class QGraphicsEllipseNode : public QGraphicsNode
{
    Q_OBJECT
public:
    enum { Type = UserType + 101 };

    QGraphicsEllipseNode(WeightedNode *node, QGraphicsItem *parent = 0);
    virtual ~QGraphicsEllipseNode();

    virtual int type() const;

private:
    QGraphicsEllipseItem    *_ellipseItem;
    QGraphicsSimpleTextItem *_idItem;
};

#endif // QGRAPHICSELLIPSENODE_H
