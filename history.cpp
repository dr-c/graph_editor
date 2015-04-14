#include "history.h"

#include "graph_scene.h"
#include "qgraphics_node.h"
#include "qgraphics_edge.h"

#include <assert.h>
#include <QDebug>

History::History(BasicGraphScene *scene)
    : _scene(scene)
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
    : NodeHistoryItem(history, sgNode), _centerPos((*sgNode)->node()->pos()), _weight((*sgNode)->node()->weight())
{

}

void History::CreateNode::undo()
{
    _history->_mainNodeItems.erase(*_sgNode);
    (*_sgNode)->deleteCompletely();
}

void History::CreateNode::redo()
{
    (*_sgNode) = _history->_scene->addNode(_centerPos, _weight);
    _history->_mainNodeItems.insert(std::make_pair(*_sgNode, _sgNode));
}

History::MoveNode::MoveNode(History *history, const SharedGNode &sgNode, const QPointF &beforeMove, const QPointF &afterMove)
    : NodeHistoryItem(history, sgNode), _beforeMove(beforeMove), _afterMove(afterMove)
{

}

void History::MoveNode::undo()
{
    (*_sgNode)->changePositionOutside(_beforeMove);
}

void History::MoveNode::redo()
{
    (*_sgNode)->changePositionOutside(_afterMove);
}

History::ChangeNodeWeight::ChangeNodeWeight(History *history, const SharedGNode &sgNode, int fromWeight, int toWeight)
    : NodeHistoryItem(history, sgNode), _beforeWeight(fromWeight), _afterWeight(toWeight)
{

}

void History::ChangeNodeWeight::undo()
{
    (*_sgNode)->changeWeightOutside(_beforeWeight);
}

void History::ChangeNodeWeight::redo()
{
    (*_sgNode)->changeWeightOutside(_afterWeight);
}

History::DeleteNode::DeleteNode(History *history, const SharedGNode &sgNode)
    : NodeHistoryItem(history, sgNode), _centerPos((*sgNode)->node()->pos()), _weight((*sgNode)->node()->weight())
{
    _ajacentEdges.reserve((*sgNode)->node()->countAdjacents());

    for (auto predecessor : (*sgNode)->node()->predecessors())
        fillAjacentEdges(predecessor);
    if (&(*sgNode)->node()->predecessors() != &(*sgNode)->node()->successors())
        for (auto successor : (*sgNode)->node()->successors())
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
    (*_sgNode) = _history->_scene->addNode(_centerPos, _weight);
    _history->_mainNodeItems.insert(std::make_pair(*_sgNode, _sgNode));
    std::for_each(_ajacentEdges.begin(), _ajacentEdges.end(), [](DeleteEdge &item){item.undo();});
}

void History::DeleteNode::redo()
{
    std::for_each(_ajacentEdges.begin(), _ajacentEdges.end(), [](DeleteEdge &item){item.incompleteRedo();});
    _history->_mainNodeItems.erase(*_sgNode);
    (*_sgNode)->deleteCompletely();
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
    _history->_mainEdgeItems.erase(*_sgEdge);
    (*_sgEdge)->deleteCompletely();
}

void History::CreateEdge::redo()
{
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
    (*_sgEdge)->changeWeightOutside(_beforeWeight);
}

void History::ChangeEdgeWeight::redo()
{
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
    _history->_mainEdgeItems.erase(*_sgEdge);
    (*_sgEdge)->deleteCompletely();
}
