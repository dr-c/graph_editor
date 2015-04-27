#include "history.h"

#include "graph_scene.h"
#include "qgraphics_node.h"
#include "qgraphics_edge.h"

#include <assert.h>

History::History(GraphScene *scene)
    : _scene(scene), _current(0)
{

}

const History::SharedGNode &History::findOrCreateSGNode(QGraphicsNode *gNode)
{
    auto iter = _mainNodeItems.lower_bound(gNode);
    if ((*iter).first != gNode)
        iter = _mainNodeItems.insert(iter, std::make_pair(gNode, std::make_shared<QGraphicsNode*>(gNode)));
    return (*iter).second;
}

const History::SharedGEdge &History::findOrCreateSGEdge(QGraphicsEdge *gEdge)
{
    auto iter = _mainEdgeItems.lower_bound(gEdge);
    if ((*iter).first != gEdge)
        iter = _mainEdgeItems.insert(iter, std::make_pair(gEdge, std::make_shared<QGraphicsEdge*>(gEdge)));
    return (*iter).second;
}

void History::addItem(HistoryItemPtr item)
{
    if (_items.size() > _current)
        _items.erase(_items.begin() + _current, _items.end());
    _items.push_back(std::move(item));
    _current++;
    emit newItemAdded();
}

void History::writeNodeCreation(QGraphicsNode *gNode)
{
    auto iter = _mainNodeItems.insert(std::make_pair(gNode, std::make_shared<QGraphicsNode*>(gNode)));
    addItem(HistoryItemPtr(new CreateNode(this, (*iter.first).second)));
}

void History::writeNodeMoving(QGraphicsNode *gNode, const QPointF &beforeMove, const QPointF &afterMove)
{
    addItem(HistoryItemPtr(new MoveNode(this, findOrCreateSGNode(gNode), beforeMove, afterMove)));
}

void History::writeNodeWeightChanging(QGraphicsNode *gNode, int fromWeight, int toWeight)
{
    addItem(HistoryItemPtr(new ChangeNodeWeight(this, findOrCreateSGNode(gNode), fromWeight, toWeight)));
}

void History::writeNodeDeletion(QGraphicsNode *gNode)
{
    addItem(HistoryItemPtr(new DeleteNode(this, findOrCreateSGNode(gNode))));
}

void History::writeGroupNodesMoving(const QList<QGraphicsItem*> &listItems, const QPointF &beforeMove, const QPointF &afterMove)
{
    addItem(HistoryItemPtr(new GroupMoveNode(this, listItems, beforeMove, afterMove)));
}

void History::prepareGroupNodesDeletion(int nodesCount)
{
    addItem(HistoryItemPtr(new GroupDeleteNode(this, nodesCount)));
}

void History::writeNodeDeletionToGroup(QGraphicsNode *gNode)
{
    assert(!_items.empty() && dynamic_cast<GroupDeleteNode*>(_items.back().get()) != nullptr);
    static_cast<GroupDeleteNode*>(_items.back().get())->addNode(findOrCreateSGNode(gNode));
}

void History::writeEdgeCreation(QGraphicsEdge *gEdge)
{
    auto edgeIter = _mainEdgeItems.insert(std::make_pair(gEdge, std::make_shared<QGraphicsEdge*>(gEdge)));
    addItem(HistoryItemPtr(new CreateEdge(this, (*edgeIter.first).second,
                           findOrCreateSGNode(gEdge->edge()->fromNode()->graphicsNode()),
                           findOrCreateSGNode(gEdge->edge()->toNode()->graphicsNode()))));
}

void History::writeEdgeWeightChanging(QGraphicsEdge *gEdge, int fromWeight, int toWeight)
{
    addItem(HistoryItemPtr(new ChangeEdgeWeight(this, findOrCreateSGEdge(gEdge), fromWeight, toWeight)));
}

void History::writeEdgeDeletion(QGraphicsEdge *gEdge)
{
    addItem(HistoryItemPtr(new DeleteEdge(this, findOrCreateSGEdge(gEdge),
                           findOrCreateSGNode(gEdge->edge()->fromNode()->graphicsNode()),
                           findOrCreateSGNode(gEdge->edge()->toNode()->graphicsNode()))));
}

bool History::undo()
{
    assert(_current > 0 && _current <= _items.size());
    _items[--_current]->undo();
    return _current > 0;
}

bool History::redo()
{
    assert(_current < _items.size());
    _items[_current++]->redo();
    return _current < _items.size();
}

bool History::canUndo() const
{
    return _current > 0;
}

bool History::canRedo() const
{
    return _current < _items.size();
}

void History::substituteGraphicsNode(QGraphicsNode *fromGNode, QGraphicsNode *toGNode)
{
    auto iter = _mainNodeItems.find(fromGNode);
    if (iter == _mainNodeItems.end())
        return;
    auto sg_node = iter->second;
    (*sg_node) = toGNode;
    _mainNodeItems.erase(iter);
    _mainNodeItems.insert(std::make_pair(*sg_node, sg_node));
}

void History::substituteGraphicsEdge(QGraphicsEdge *fromGEdge, QGraphicsEdge *toGEdge)
{
    auto iter = _mainEdgeItems.find(fromGEdge);
    if (iter != _mainEdgeItems.end())
        return;
    auto sg_edge= iter->second;
    (*sg_edge) = toGEdge;
    _mainEdgeItems.erase(iter);
    _mainEdgeItems.insert(std::make_pair(*sg_edge, sg_edge));
}

History::HistoryItem::HistoryItem(History *history)
    : _history(history)
{

}

History::NodeHistoryItem::NodeHistoryItem(History *history, const SharedGNode &sgNode)
    : HistoryItem(history), _sgNode(sgNode)
{

}

History::NodeHistoryItem::~NodeHistoryItem()
{
    auto iter = _history->_mainNodeItems.find(*_sgNode);
    if (iter == _history->_mainNodeItems.end())
        return;
    _sgNode.reset();
    if ((*iter).second.unique())
        _history->_mainNodeItems.erase(iter);
}

History::EdgeHistoryItem::EdgeHistoryItem(History *history, const SharedGEdge &sgEdge)
    : HistoryItem(history), _sgEdge(sgEdge)
{

}

History::EdgeHistoryItem::~EdgeHistoryItem()
{
    auto iter = _history->_mainEdgeItems.find(*_sgEdge);
    if (iter == _history->_mainEdgeItems.end())
        return;
    _sgEdge.reset();
    if ((*iter).second.unique())
        _history->_mainEdgeItems.erase(iter);
}

History::CreateNode::CreateNode(History *history, const SharedGNode &sgNode)
    : NodeHistoryItem(history, sgNode), _centerPos((*sgNode)->node()->pos()),
      _weight((*sgNode)->node()->weight()), _id((*sgNode)->node()->id())
{

}

void History::CreateNode::undo()
{
    _history->_scene->clearSelection();
    _history->_mainNodeItems.erase(*_sgNode);
    (*_sgNode)->deleteCompletely();
}

void History::CreateNode::redo()
{
    _history->_scene->clearSelection();
    (*_sgNode) = _history->_scene->addNode(_centerPos, _weight, _id);
    _history->_mainNodeItems.insert(std::make_pair(*_sgNode, _sgNode));
    (*_sgNode)->setSelected(true);
}

History::MoveNode::MoveNode(History *history, const SharedGNode &sgNode, const QPointF &beforeMove, const QPointF &afterMove)
    : NodeHistoryItem(history, sgNode), _beforeMove(beforeMove), _afterMove(afterMove)
{

}

void History::MoveNode::undo()
{
    _history->_scene->clearSelection();
    undoKeepSelection();
}

void History::MoveNode::redo()
{
    _history->_scene->clearSelection();
    redoKeepSelection();
}


void History::MoveNode::undoKeepSelection()
{
    (*_sgNode)->changePositionOutside(_beforeMove);
    (*_sgNode)->setSelected(true);
}

void History::MoveNode::redoKeepSelection()
{
    (*_sgNode)->changePositionOutside(_afterMove);
    (*_sgNode)->setSelected(true);
}

History::ChangeNodeWeight::ChangeNodeWeight(History *history, const SharedGNode &sgNode, int fromWeight, int toWeight)
    : NodeHistoryItem(history, sgNode), _beforeWeight(fromWeight), _afterWeight(toWeight)
{

}

void History::ChangeNodeWeight::undo()
{
    _history->_scene->clearSelection();
    (*_sgNode)->changeWeightOutside(_beforeWeight);
    (*_sgNode)->setSelected(true);
}

void History::ChangeNodeWeight::redo()
{
    _history->_scene->clearSelection();
    (*_sgNode)->changeWeightOutside(_afterWeight);
    (*_sgNode)->setSelected(true);
}

History::DeleteNode::DeleteNode(History *history, const SharedGNode &sgNode)
    : NodeHistoryItem(history, sgNode), _centerPos((*sgNode)->node()->pos()),
      _weight((*sgNode)->node()->weight()), _id((*sgNode)->node()->id())
{
    _ajacentEdges.reserve((*sgNode)->node()->countAdjacents());

    for (auto& predecessor : (*sgNode)->node()->predecessors())
        fillAjacentEdges(predecessor);
    if (&(*sgNode)->node()->predecessors() != &(*sgNode)->node()->successors())
        for (auto& successor : (*sgNode)->node()->successors())
            fillAjacentEdges(successor);

    _history->_mainNodeItems.erase(*_sgNode);
}

void History::DeleteNode::fillAjacentEdges(const std::pair<WeightedNode* const, WeightedEdge*>& pair)
{
    QGraphicsEdge *graphics_edge = pair.second->graphicsEdge();
    _ajacentEdges.emplace_back(_history, _history->findOrCreateSGEdge(graphics_edge),
                               _history->findOrCreateSGNode(graphics_edge->edge()->fromNode()->graphicsNode()),
                               _history->findOrCreateSGNode(graphics_edge->edge()->toNode()->graphicsNode()));
}

void History::DeleteNode::undo()
{
    _history->_scene->clearSelection();
    undoNodeOnly();
    undoEdgesOnly();
}

void History::DeleteNode::redo()
{
    _history->_scene->clearSelection();
    for (auto& item : _ajacentEdges)
        item.incompleteRedo();
    _history->_mainNodeItems.erase(*_sgNode);
    (*_sgNode)->deleteCompletely();
}

void History::DeleteNode::undoNodeOnly()
{
    (*_sgNode) = _history->_scene->addNode(_centerPos, _weight, _id);
    _history->_mainNodeItems.insert(std::make_pair(*_sgNode, _sgNode));
    (*_sgNode)->setSelected(true);
}

void History::DeleteNode::undoEdgesOnly()
{
    for (auto& item : _ajacentEdges)
        item.undo();
}

History::CreateEdge::CreateEdge(History *history, const SharedGEdge &sgEdge, const SharedGNode &fromSGNode, const SharedGNode &toSGNode)
    : EdgeHistoryItem(history, sgEdge), _weight((*_sgEdge)->edge()->weight()), _fromSGNode(fromSGNode), _toSGNode(toSGNode)
{

}

History::CreateEdge::~CreateEdge()
{
    auto iter = _history->_mainNodeItems.find(*_fromSGNode);
    _fromSGNode.reset();
    if ((*iter).second.unique())
        _history->_mainNodeItems.erase(iter);

    iter = _history->_mainNodeItems.find(*_toSGNode);
    _toSGNode.reset();
    if ((*iter).second.unique())
        _history->_mainNodeItems.erase(iter);
}

void History::CreateEdge::undo()
{
    _history->_scene->clearSelection();
    _history->_mainEdgeItems.erase(*_sgEdge);
    (*_sgEdge)->deleteCompletely();
}

void History::CreateEdge::redo()
{
    _history->_scene->clearSelection();
    (*_sgEdge) = _history->_scene->addEdge(_weight);
    (*_sgEdge)->join(*_fromSGNode, *_toSGNode);
    (*_sgEdge)->showWeight();
    _history->_mainEdgeItems.insert(std::make_pair(*_sgEdge, _sgEdge));
}

History::ChangeEdgeWeight::ChangeEdgeWeight(History *history, const SharedGEdge &sgEdge, int fromWeight, int toWeight)
    : EdgeHistoryItem(history, sgEdge), _beforeWeight(fromWeight), _afterWeight(toWeight)
{

}

void History::ChangeEdgeWeight::undo()
{
    _history->_scene->clearSelection();
    (*_sgEdge)->changeWeightOutside(_beforeWeight);
}

void History::ChangeEdgeWeight::redo()
{
    _history->_scene->clearSelection();
    (*_sgEdge)->changeWeightOutside(_afterWeight);
}

History::DeleteEdge::DeleteEdge(History *history, const SharedGEdge &sgEdge, const SharedGNode &fromSGNode, const SharedGNode &toSGNode)
    : EdgeHistoryItem(history, sgEdge), _weight((*_sgEdge)->edge()->weight()), _fromSGNode(fromSGNode), _toSGNode(toSGNode)
{

}

History::DeleteEdge::~DeleteEdge()
{
    auto iter = _history->_mainNodeItems.find(*_fromSGNode);
    _fromSGNode.reset();
    if ((*iter).second.unique())
        _history->_mainNodeItems.erase(iter);

    iter = _history->_mainNodeItems.find(*_toSGNode);
    _toSGNode.reset();
    if ((*iter).second.unique())
        _history->_mainNodeItems.erase(iter);
}

void History::DeleteEdge::undo()
{
    _history->_scene->clearSelection();
    (*_sgEdge) = _history->_scene->addEdge(_weight);
    (*_sgEdge)->join(*_fromSGNode, *_toSGNode);
    (*_sgEdge)->showWeight();
    _history->_mainEdgeItems.insert(std::make_pair(*_sgEdge, _sgEdge));
}

void History::DeleteEdge::incompleteRedo()
{
    _history->_mainEdgeItems.erase(*_sgEdge);
}

void History::DeleteEdge::redo()
{
    _history->_scene->clearSelection();
    _history->_mainEdgeItems.erase(*_sgEdge);
    (*_sgEdge)->deleteCompletely();
}

History::GroupMoveNode::GroupMoveNode(History *history, const QList<QGraphicsItem *> &items,
                                      const QPointF &beforeMove, const QPointF &afterMove)
    : HistoryItem(history)
{
    _moveNodes.reserve(items.count());
    QPointF shift = afterMove - beforeMove;
    for (auto item : items)
    {
        assert(dynamic_cast<QGraphicsNode*>(item) != nullptr);
        const QPointF &cur_pos = item->pos();
        _moveNodes.emplace_back(history, _history->findOrCreateSGNode(static_cast<QGraphicsNode*>(item)),
                                cur_pos - shift, cur_pos);
    }
}

void History::GroupMoveNode::undo()
{
    _history->_scene->clearSelection();
    for (auto& item : _moveNodes)
        item.undoKeepSelection();
}

void History::GroupMoveNode::redo()
{
    _history->_scene->clearSelection();
    for (auto& item : _moveNodes)
        item.redoKeepSelection();
}

History::GroupDeleteNode::GroupDeleteNode(History *history, int deleteNodesCount)
    : HistoryItem(history)
{
    _deleteNodes.reserve(deleteNodesCount);
}

void History::GroupDeleteNode::addNode(const SharedGNode &sgNode)
{
    _deleteNodes.emplace_back(_history, sgNode);
}

void History::GroupDeleteNode::undo()
{
    assert(_deleteNodes.capacity() == _deleteNodes.size());
    _history->_scene->clearSelection();
    for (auto& item : _deleteNodes)
        item.undoNodeOnly();
    for (auto& item : _deleteNodes)
        item.undoEdgesOnly();
}

void History::GroupDeleteNode::redo()
{
    assert(_deleteNodes.capacity() == _deleteNodes.size());
    _history->_scene->clearSelection();
    for (auto& item : _deleteNodes)
        item.redo();
}
