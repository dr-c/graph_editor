#ifndef GRAPHSCENEMODE_H
#define GRAPHSCENEMODE_H

class BasicGraphScene;
class QGraphicsItem;

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
    PointerMode(GraphScene *graphScene);
    virtual ~PointerMode();

    virtual void    setItemFlags(QGraphicsItem *item);
};

class PencilMode : public GraphSceneMode
{
public:
    PencilMode(GraphScene *graphScene) : GraphSceneMode(graphScene), _itemFrom(nullptr), _itemMousePress(nullptr), _itemArrow(nullptr) {}
    virtual ~PencilMode();

    virtual void    setItemFlags(QGraphicsItem *item);
};

#endif // GRAPHSCENEMODE_H
