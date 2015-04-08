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

PointerMode::PointerMode(BasicGraphScene *graphScene)
    : GraphSceneMode(graphScene)
{

}

PointerMode::~PointerMode()
{

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

void PencilMode::setItemFlags(QGraphicsItem *item)
{
    if (item->type() == _scene->typeGraphicsNode())
    {
        item->setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemContainsChildrenInShape);
    }
}
