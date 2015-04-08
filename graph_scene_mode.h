#ifndef GRAPHSCENEMODE_H
#define GRAPHSCENEMODE_H

#include <QtGlobal>

class BasicGraphScene;
class QGraphicsItem;
class QGraphicsSceneMouseEvent;
class QKeyEvent;

class GraphSceneMode
{
public:
    virtual ~GraphSceneMode();

    virtual void    setItemFlags(QGraphicsItem *item) = 0;

    virtual void    mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)   { Q_UNUSED(mouseEvent); }
    virtual void    mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)    { Q_UNUSED(mouseEvent); }
    virtual void	mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) { Q_UNUSED(mouseEvent); }
    virtual void	keyPressEvent(QKeyEvent *keyEvent)                      { Q_UNUSED(keyEvent);   }
    virtual void    keyReleaseEvent(QKeyEvent *keyEvent)                    { Q_UNUSED(keyEvent);   }

    BasicGraphScene *scene();

protected:
    GraphSceneMode(BasicGraphScene *scene);

    BasicGraphScene *_scene;
};

class PointerMode : public GraphSceneMode
{
public:
    PointerMode(BasicGraphScene *graphScene);
    virtual ~PointerMode();

    virtual void    setItemFlags(QGraphicsItem *item);
};

class PencilMode : public GraphSceneMode
{
public:
    PencilMode(BasicGraphScene *graphScene);
    virtual ~PencilMode();

    virtual void    setItemFlags(QGraphicsItem *item);
};

#endif // GRAPHSCENEMODE_H
