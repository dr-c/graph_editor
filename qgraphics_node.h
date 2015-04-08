#ifndef QGRAPHICSNODE_H
#define QGRAPHICSNODE_H

#include <QGraphicsObject>

#include "item_info.h"

class QGraphicsNode : public QGraphicsObject
{
    Q_OBJECT
public:
    enum { Type = UserType + 100 };

    virtual ~QGraphicsNode();

    virtual int type() const;

protected:
    QGraphicsNode(WeightedNode *node, QGraphicsItem *parent = 0);

    WeightedNode *_node;
};

#endif // QGRAPHICSNODE_H
