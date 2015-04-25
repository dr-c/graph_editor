/*
 * The History is a class which stores change records on Scene.
 *
 * Types of supported change records:
 *  - creation, moving, weight changing and deletion of Nodes;
 *  - creation, weight changing and deletion of Edges;
 *  - moving and deletion a Node groups.
 * All records store in list of records(actually, vector).
 *
 * write...() functions create record in history and emit newItemAdded signal.
 * If some records follows active record in the list of records, they will be delete.
 *
 * undo() erases the last change done to the document reverting it to an older state.
 * redo() reverts the effects of the undo() action.
 * undo() moves active record back, and redo() - forward in the list of records.
 *
 * canUndo()/canRedo() function check, is it possible to use undo/redo() functions.
 */

#ifndef HISTORY_H
#define HISTORY_H

#include <QObject>
#include <vector>
#include <map>
#include <memory>
#include <QPointF>
#include "item_info.h"

class GraphScene;
class QGraphicsItem;
class QGraphicsNode;
class QGraphicsEdge;

class History : public QObject
{
    Q_OBJECT
public:
    History(GraphScene *scene);
    ~History() = default;
    void writeNodeCreation(QGraphicsNode *gNode);
    void writeNodeMoving(QGraphicsNode *gNode, const QPointF &beforeMove, const QPointF &afterMove);
    void writeNodeWeightChanging(QGraphicsNode *gNode, int fromWeight, int toWeight);
    void writeNodeDeletion(QGraphicsNode *gNode);
    void writeGroupNodesMoving(const QList<QGraphicsItem *> &listItems, const QPointF &beforeMove, const QPointF &afterMove);
    void prepareGroupNodesDeletion(int nodesCount);
    void writeNodeDeletionToGroup(QGraphicsNode *gNode);
    void writeEdgeCreation(QGraphicsEdge *gEdge);
    void writeEdgeWeightChanging(QGraphicsEdge *gEdge, int fromWeight, int toWeight);
    void writeEdgeDeletion(QGraphicsEdge *gEdge);
    bool undo();
    bool redo();
    bool canUndo() const;
    bool canRedo() const;

signals:
    void newItemAdded();

private:
    class HistoryItem;

    typedef std::unique_ptr<HistoryItem>    HistoryItemPtr;
    typedef std::shared_ptr<QGraphicsNode*> SharedGNode;
    typedef std::shared_ptr<QGraphicsEdge*> SharedGEdge;

    const SharedGNode &findOrCreateSGNode(QGraphicsNode *gNode);
    const SharedGEdge &findOrCreateSGEdge(QGraphicsEdge *gEdge);

    void addItem(HistoryItemPtr item);

    std::map<QGraphicsNode*, SharedGNode> _mainNodeItems;
    std::map<QGraphicsEdge*, SharedGEdge> _mainEdgeItems;

    GraphScene            *_scene;
    std::size_t                 _current;

    std::vector<HistoryItemPtr> _items;

    class HistoryItem {
    public:
        HistoryItem(History *history);
        virtual ~HistoryItem() = default;
        virtual void undo() = 0;
        virtual void redo() = 0;
    protected:
        History *_history;
    };
    class NodeHistoryItem : public HistoryItem {
    public:
        NodeHistoryItem(History *history, const SharedGNode &sgNode);
        virtual ~NodeHistoryItem() override;
    protected:
        SharedGNode _sgNode;
    };
    class EdgeHistoryItem : public HistoryItem {
    public:
        EdgeHistoryItem(History *history, const SharedGEdge &sgEdge);
        virtual ~EdgeHistoryItem() override;
    protected:
        SharedGEdge _sgEdge;
    };

    class CreateNode : public NodeHistoryItem {
    public:
        CreateNode(History *history, const SharedGNode &sgNode);
        virtual ~CreateNode() override = default;
        virtual void undo() override;
        virtual void redo() override;
    private:
        QPointF _centerPos;
        int     _weight;
        int     _id;
    };
    class MoveNode : public NodeHistoryItem {
    public:
        MoveNode(History *history, const SharedGNode &sgNode, const QPointF &beforeMove, const QPointF &afterMove);
        virtual ~MoveNode() override = default;
        virtual void undo() override;
        virtual void redo() override;
        void undoKeepSelection();
        void redoKeepSelection();
    private:
        QPointF         _beforeMove;
        QPointF         _afterMove;
    };
    class ChangeNodeWeight : public NodeHistoryItem {
    public:
        ChangeNodeWeight(History *history, const SharedGNode &sgNode, int fromWeight, int toWeight);
        virtual ~ChangeNodeWeight() override = default;
        virtual void undo() override;
        virtual void redo() override;
    private:
        int _beforeWeight;
        int _afterWeight;
    };
    class DeleteEdge;
    class DeleteNode : public NodeHistoryItem {
    public:
        DeleteNode(History *history, const SharedGNode &sgNode);
        virtual ~DeleteNode() override = default;
        virtual void undo() override;
        virtual void redo() override;
        void undoNodeOnly();
        void undoEdgesOnly();
        void fillAjacentEdges(const std::pair<WeightedNode * const, WeightedEdge *> &pair);
    private:
        std::vector<DeleteEdge> _ajacentEdges;
        QPointF _centerPos;
        int     _weight;
        int     _id;
    };
    class CreateEdge : public EdgeHistoryItem {
    public:
        CreateEdge(History *history, const SharedGEdge &sgEdge, const SharedGNode &fromSGNode, const SharedGNode &toSGNode);
        virtual ~CreateEdge() override;
        virtual void undo() override;
        virtual void redo() override;
    private:
        int         _weight;
        SharedGNode _fromSGNode;
        SharedGNode _toSGNode;
    };
    class ChangeEdgeWeight : public EdgeHistoryItem {
    public:
        ChangeEdgeWeight(History *history, const SharedGEdge &sgEdge, int fromWeight, int toWeight);
        virtual ~ChangeEdgeWeight() override = default;
        virtual void undo() override;
        virtual void redo() override;
    private:
        int _beforeWeight;
        int _afterWeight;
    };
    class DeleteEdge : public EdgeHistoryItem {
    public:
        DeleteEdge(History *history, const SharedGEdge &sgEdge, const SharedGNode &fromSGNode, const SharedGNode &toSGNode);
        virtual ~DeleteEdge() override;
        virtual void undo() override;
        virtual void redo() override;
        void incompleteRedo();
    private:
        int         _weight;
        SharedGNode _fromSGNode;
        SharedGNode _toSGNode;
    };

    class GroupMoveNode : public HistoryItem
    {
    public:
        GroupMoveNode(History *history, const QList<QGraphicsItem *> &items, const QPointF &beforeMove, const QPointF &afterMove);
        virtual ~GroupMoveNode() override = default;
        virtual void undo() override;
        virtual void redo() override;
    protected:
        std::vector<MoveNode> _moveNodes;
    };
    class GroupDeleteNode : public HistoryItem
    {
    public:
        GroupDeleteNode(History *history, int deleteNodesCount);
        virtual ~GroupDeleteNode() override = default;
        virtual void undo() override;
        virtual void redo() override;
        void addNode(const SharedGNode &sgNode);
    protected:
        std::vector<DeleteNode> _deleteNodes;
    };
};

#endif // HISTORY_H
