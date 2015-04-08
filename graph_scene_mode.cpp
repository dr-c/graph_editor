#include "graph_scene_mode.h"

#include "graph_scene.h"

#include <QGraphicsItem>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

GraphSceneMode::GraphSceneMode(BasicGraphScene *scene)
    : _scene(scene)
{

}

GraphSceneMode::~GraphSceneMode()
{

}

void GraphSceneMode::setScene(BasicGraphScene *scene)
{
    _scene = scene;
}

BasicGraphScene *GraphSceneMode::scene()
{
    return _scene;
}

PointerMode::PointerMode(BasicGraphScene *graphScene)
    : GraphSceneMode(graphScene)
{

}

PointerMode::~PointerMode()
{

}

int PointerMode::type() const
{
    return Type;
}

void PointerMode::setItemFlags(QGraphicsItem *item)
{
    if (item->type() == _scene->typeGraphicsNode())
    {
        item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable
                            | QGraphicsItem::ItemContainsChildrenInShape | QGraphicsItem::ItemSendsScenePositionChanges);
    }
}

PencilMode::PencilMode(BasicGraphScene *graphScene)
    : GraphSceneMode(graphScene)
{

}

PencilMode::~PencilMode()
{

}

int PencilMode::type() const
{
    return Type;
}

void PencilMode::setItemFlags(QGraphicsItem *item)
{
    if (item->type() == _scene->typeGraphicsNode())
    {
        item->setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemContainsChildrenInShape);
    }
}
