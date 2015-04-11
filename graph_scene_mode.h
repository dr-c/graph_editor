#ifndef GRAPHSCENEMODE_H
#define GRAPHSCENEMODE_H

#include <QtGlobal>
#include <QPointF>

class BasicGraphScene;
class QGraphicsItem;
class QGraphicsNode;
class QGraphicsEdge;

class QGraphicsSceneMouseEvent;
class QKeyEvent;

class GraphSceneMode
{    
public:
    virtual ~GraphSceneMode();

    virtual void    setItemFlags(QGraphicsItem *item) = 0;
    virtual int     type() const = 0;

    virtual void    mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void    mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)       { Q_UNUSED(mouseEvent); }
    virtual void    mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)        { Q_UNUSED(mouseEvent); }
    virtual void	mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)     { Q_UNUSED(mouseEvent); }
    virtual void	keyPressEvent(QKeyEvent *keyEvent)                          { Q_UNUSED(keyEvent);   }
    virtual void    keyReleaseEvent(QKeyEvent *keyEvent)                        { Q_UNUSED(keyEvent);   }

    BasicGraphScene *scene();

protected:
    GraphSceneMode(BasicGraphScene *scene = nullptr);

    BasicGraphScene *_scene;

private:
    void setScene(BasicGraphScene *scene);

    friend class BasicGraphScene;
};

class PointerMode : public GraphSceneMode
{
public:
    enum { Type = 1 };
    PointerMode(BasicGraphScene *graphScene = nullptr);
    virtual ~PointerMode();

    virtual int     type() const;
    virtual void    setItemFlags(QGraphicsItem *item);
};

class PencilMode : public GraphSceneMode
{
public:
    enum { Type = 2 };
    PencilMode(BasicGraphScene *graphScene = nullptr);
    virtual ~PencilMode();

    virtual void    setItemFlags(QGraphicsItem *item);
    virtual int     type() const;

    virtual void    mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    virtual void    mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    virtual void    mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    virtual void	mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    virtual void	keyPressEvent(QKeyEvent *keyEvent) override;

private:
    static QGraphicsItem *findNodeInAncestors(QGraphicsItem *item, int type);
    static void toggleAcceptHoverEvent(QGraphicsNode *gNode, bool enable);

    void reset();

    QPointF         _mousePressedPoint;
    QGraphicsNode  *_mousePressedItem;
    QGraphicsNode  *_firstClickedItem;
    QGraphicsEdge  *_arrowItem;
};

#endif // GRAPHSCENEMODE_H
