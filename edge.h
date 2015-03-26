#ifndef EDGE_H
#define EDGE_H


class Edge
{
protected:
    Edge();
    virtual ~Edge();
};

class DirectedEdge : public Edge
{
public:
    DirectedEdge();
    virtual ~DirectedEdge();
};

class UndirectedEdge : public Edge
{
public:
    UndirectedEdge();
    virtual ~UndirectedEdge();
};

#endif // EDGE_H
