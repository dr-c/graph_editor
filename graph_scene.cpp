#include "graph_scene.h"

GraphScene::GraphScene(Graph<NodeInfo, EdgeInfo> *graph, QObject *parent)
    : QGraphicsScene(parent), _graph(graph)
{

}

GraphScene::~GraphScene()
{
    delete _graph;
}

DirectedGraphScene::DirectedGraphScene(QObject *parent = 0)
    : GraphScene(parent, new DirectedGraph<NodeInfo, EdgeInfo>())
{

}

DirectedGraphScene::~DirectedGraphScene()
{

}

UndirectedGraphScene::UndirectedGraphScene(QObject *parent = 0)
    : GraphScene(parent, new UndirectedGraph<NodeInfo, EdgeInfo>())
{

}

UndirectedGraphScene::~UndirectedGraphScene()
{

}
