#include "graph_scene_mode.h"

#include "graph_scene.h"
#include <QGraphicsItem>

GraphSceneMode::GraphSceneMode(BasicGraphScene *scene)
    : _scene(scene)
{

}

GraphSceneMode::~GraphSceneMode()
{

}

PointerMode::PointerMode(GraphScene *graphScene)
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

PencilMode::PencilMode(GraphScene *graphScene)
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
