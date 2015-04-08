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
    virtual int     type() const = 0;

    virtual void    mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)   { Q_UNUSED(mouseEvent); }
    virtual void    mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)    { Q_UNUSED(mouseEvent); }
    virtual void	mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) { Q_UNUSED(mouseEvent); }
    virtual void	keyPressEvent(QKeyEvent *keyEvent)                      { Q_UNUSED(keyEvent);   }
    virtual void    keyReleaseEvent(QKeyEvent *keyEvent)                    { Q_UNUSED(keyEvent);   }

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

    virtual int     type() const;
    virtual void    setItemFlags(QGraphicsItem *item);
};

#endif // GRAPHSCENEMODE_H
