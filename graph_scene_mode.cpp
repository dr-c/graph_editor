#include "graph_scene_mode.h"

#include "graph_scene.h"
#include "qgraphics_node.h"
#include "qgraphics_edge.h"

#include <QGraphicsItem>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

GraphSceneMode::GraphSceneMode(BasicGraphScene *scene)
    : _scene(scene)
{

}

void GraphSceneMode::setScene(BasicGraphScene *scene)
{
    _scene = scene;
}

const BasicGraphScene *GraphSceneMode::scene() const
{
    return _scene;
}

void GraphSceneMode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton && _scene->itemAt(mouseEvent->scenePos(), QTransform()) == nullptr)
        _scene->history()->writeNodeCreation(_scene->addNode(mouseEvent->scenePos()));
}

PointerMode::PointerMode(BasicGraphScene *graphScene)
    : GraphSceneMode(graphScene)
{

}

int PointerMode::type() const
{
    return Type;
}

void PointerMode::setGNodeFlags(QGraphicsNode *gNode) const
{
    gNode->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable
                        | QGraphicsItem::ItemContainsChildrenInShape | QGraphicsItem::ItemSendsScenePositionChanges);
}

void PointerMode::keyPressEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->key() == Qt::Key_Delete)
    {
        const QList<QGraphicsItem*> &listItems = _scene->selectedItems();
        if (listItems.count() < 2)
            return;
        _scene->history()->prepareGroupNodesDeletion(listItems.count());
        for (auto item : listItems)
        {
            assert(dynamic_cast<QGraphicsNode*>(item) != nullptr);
            QGraphicsNode* graphics_node = static_cast<QGraphicsNode*>(item);
            _scene->history()->writeNodeDeletionToGroup(graphics_node);
            graphics_node->deleteCompletely();
        }
    }
}

PencilMode::PencilMode(BasicGraphScene *graphScene)
    : GraphSceneMode(graphScene),
      _mousePressedItem(nullptr),
      _firstClickedItem(nullptr),
      _arrowItem(nullptr)
{

}

PencilMode::~PencilMode()
{
    if (_arrowItem != nullptr)
        _arrowItem->deleteCompletely();
}

int PencilMode::type() const
{
    return Type;
}

void PencilMode::setGNodeFlags(QGraphicsNode *gNode) const
{
    gNode->setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemContainsChildrenInShape);
}

void PencilMode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (_arrowItem == nullptr)
        GraphSceneMode::mouseDoubleClickEvent(mouseEvent);
    else
    {
        if (findNodeInAncestors(_scene->itemAt(mouseEvent->scenePos(), QTransform()), _scene->typeGraphicsNode()) != nullptr)
        {
            _arrowItem->deleteCompletely();
            reset();
        }
    }
}

void PencilMode::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsItem *cur_item = findNodeInAncestors(_scene->itemAt(mouseEvent->scenePos(), QTransform()), _scene->typeGraphicsNode());
    if (cur_item != nullptr)
    {
        _mousePressedPoint = mouseEvent->scenePos();
        _mousePressedItem = static_cast<QGraphicsNode*>(cur_item);
        if (_firstClickedItem == nullptr)
            toggleAcceptHoverEvent(_mousePressedItem, false);
        assert(_mousePressedItem != nullptr);
    }
}

void PencilMode::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (_mousePressedItem != nullptr &&
            (abs(_mousePressedPoint.x() - mouseEvent->scenePos().x()) > 5 ||
             abs(_mousePressedPoint.y() - mouseEvent->scenePos().y()) > 5))
    {
        if (_firstClickedItem == nullptr)
            toggleAcceptHoverEvent(_mousePressedItem, true);
        _mousePressedItem = nullptr;
    }

    if (_firstClickedItem != nullptr)
    {
        QPointF cur_point = mouseEvent->scenePos();
        QGraphicsItem *cur_item = findNodeInAncestors(_scene->itemAt(cur_point, QTransform()), _scene->typeGraphicsNode());
        if (cur_item == _firstClickedItem)
        {
            if (_arrowItem->isVisible())
                _arrowItem->hide();
            return;
        }
        else
        {
            if (!_arrowItem->isVisible())
                _arrowItem->show();
        }

        if (cur_item == nullptr)
            _arrowItem->draw(_firstClickedItem, cur_point);
        else
        {
            QGraphicsNode *cur_gnode = static_cast<QGraphicsNode*>(cur_item);
            if (_firstClickedItem->node()->hasSuccessor(cur_gnode->node()))
                _arrowItem->draw(_firstClickedItem, cur_point);
            else
                _arrowItem->draw(_firstClickedItem, cur_gnode);
        }
    }
}

void PencilMode::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    Q_UNUSED(mouseEvent);
    if (_mousePressedItem == nullptr)
        return;

    if (_firstClickedItem == nullptr)
    {
        _firstClickedItem = _mousePressedItem;
        _arrowItem = _scene->addEdge();
        _arrowItem->setFocus();
    }
    else
    {
        if (_firstClickedItem != _mousePressedItem && !_firstClickedItem->node()->hasSuccessor(_mousePressedItem->node()))
        {
            toggleAcceptHoverEvent(_firstClickedItem, true);
            _arrowItem->join(_firstClickedItem, _mousePressedItem);
            _arrowItem->showWeight();
            _arrowItem->setAcceptHoverEvents(true);
            _scene->history()->writeEdgeCreation(_arrowItem);
            reset();
        }
    }
}

void PencilMode::keyPressEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->key() == Qt::Key_Escape && _arrowItem != nullptr)
    {
        toggleAcceptHoverEvent(_firstClickedItem, true);
        _arrowItem->deleteCompletely();
        reset();
    }
}

void PencilMode::reset()
{
    _mousePressedItem = nullptr;
    _firstClickedItem = nullptr;
    _arrowItem = nullptr;
}

QGraphicsItem *PencilMode::findNodeInAncestors(QGraphicsItem *item, int type)
{
    while (item != nullptr && item->type() != type)
        item = item->parentItem();
    return item;
}

void PencilMode::toggleAcceptHoverEvent(QGraphicsNode *gNode, bool enable)
{
    if (enable)
        gNode->setActivePen(gNode->pen());

    gNode->setAcceptHoverEvents(enable);
    for (auto pair : gNode->node()->successors())
        pair.first->graphicsNode()->setAcceptHoverEvents(enable);
}
