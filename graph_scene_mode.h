/*
 * GraphSceneMode determines behavior for QGraphicsNode via flags (setGNodeFlags(gNode*))
 *  and presents interaction for Scene via events.
 *
 * PointerMode: Nodes can be moved, selected and focused(accept key events after click).
 * MouseDoubleClick creates new Node.
 *
 * PrencilMode: Nodes cann`t be moved and selected, only focused(accept key events after click).
 * MouseDoubleClick creates new Node(if no active Edge)
 *  or deletes active Edge(if click was on the Node, from which it Edge begins)
 *   or does nothing(if exists active Edge and click was on free space.)
 * For creating new Edge click(Press+Release mouse button) on one Node and another.
 * If there another Edge between these Nodes, Edge wouldn`t be created.
 * If Escape_key pressed, while connecting two Nodes, Edge should be deleted.
 */

#ifndef GRAPHSCENEMODE_H
#define GRAPHSCENEMODE_H

#include <QtGlobal>
#include <QPointF>

class GraphScene;
class QGraphicsItem;
class QGraphicsNode;
class QGraphicsEdge;

class QGraphicsSceneMouseEvent;
class QKeyEvent;

class GraphSceneMode
{    
public:
    virtual ~GraphSceneMode() = default;

    virtual void    setGNodeFlags(QGraphicsNode *gNode) const = 0;
    virtual int     type() const = 0;

    virtual void    mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void    mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)   { Q_UNUSED(mouseEvent); }
    virtual void    mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)    { Q_UNUSED(mouseEvent); }
    virtual void	mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) { Q_UNUSED(mouseEvent); }
    virtual void	keyPressEvent(QKeyEvent *keyEvent)                      { Q_UNUSED(keyEvent);   }
    virtual void    keyReleaseEvent(QKeyEvent *keyEvent)                    { Q_UNUSED(keyEvent);   }

    const GraphScene *scene() const;

protected:
    GraphSceneMode(GraphScene *scene = nullptr);

    GraphScene *_scene;

private:
    friend class GraphScene;
    void setScene(GraphScene *scene);
};

class PointerMode : public GraphSceneMode
{
public:
    enum { Type = 1 };
    PointerMode(GraphScene *graphScene = nullptr);
    virtual ~PointerMode() override = default;

    virtual int     type() const override;
    virtual void    setGNodeFlags(QGraphicsNode *gNode) const override;
    virtual void	keyPressEvent(QKeyEvent *keyEvent) override;
};

class PencilMode : public GraphSceneMode
{
public:
    enum { Type = 2 };
    PencilMode(GraphScene *graphScene = nullptr);
    virtual ~PencilMode() override;

    virtual int     type() const override;
    virtual void    setGNodeFlags(QGraphicsNode *gNode) const override;

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
