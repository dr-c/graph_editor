#ifndef NODE_H
#define NODE_H

class Graph;
class Edge;

class QPointF;

class Node
{
public:
    Node(Graph *graph, int id, int weight, const QPointF &pos = QPointF());
    ~Node();

    void setWeight(int weight);
    void setPos(const QPointF &pos);

    int     id()        const;
    int     weight()    const;
    QPointF pos()       const;

private:
    Graph   *_graph;
    int      _id;
    int      _weight;
    QPointF  _pos;
};

#endif // NODE_H
