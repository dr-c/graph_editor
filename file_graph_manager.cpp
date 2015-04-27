#include "file_graph_manager.h"

#include <QPen>
#include <QFile>
#include <QXmlStreamWriter>
#include <QDomDocument>

#include "graph_scene.h"
#include "qgraphics_simple_line_edge.h"
#include "qgraphics_cubic_arrow_edge.h"
#include "qgraphics_ellipse_node.h"
#include "qgraphics_rounded_rect_node.h"

void FileGraphManager::save(QFile &file, GraphScene *scene)
{
    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
     stream.writeStartElement("Graph");
      stream.writeAttribute("isDirected", scene->graph()->isDirected() ? "true" : "false");

      stream.writeStartElement("Design");
       stream.writeStartElement("Types");
        stream.writeAttribute("node", QString::number(scene->config()->_nodeCreator->type()));
        stream.writeAttribute("edge", QString::number(scene->config()->_edgeCreator->type()));
       stream.writeEndElement();        // Types

       stream.writeStartElement("Pens");
        stream.writeStartElement("Simple");
         stream.writeStartElement("Node");
          writePenToXML(&stream, scene->config()->_nodePen);
         stream.writeEndElement();              // Node
         stream.writeStartElement("Edge");
          writePenToXML(&stream, scene->config()->_edgePen);
         stream.writeEndElement();              // Edge
        stream.writeEndElement();           // Simple

        stream.writeStartElement("Hover");
         stream.writeStartElement("Node");
          writePenToXML(&stream, scene->config()->_nodeHoverPen);
         stream.writeEndElement();              // Node
         stream.writeStartElement("Edge");
          writePenToXML(&stream, scene->config()->_edgeHoverPen);
         stream.writeEndElement();              // Edge
        stream.writeEndElement();           // Hover
       stream.writeEndElement();        // Pens

       stream.writeStartElement("Brushes");
        stream.writeStartElement("Node");
         writeBrushToXML(&stream, scene->config()->_nodeBrush);
        stream.writeEndElement();              // Node
        stream.writeStartElement("Edge");
         writeBrushToXML(&stream, scene->config()->_edgeBrush);
        stream.writeEndElement();              // Edge
       stream.writeEndElement();        // Brushes

       stream.writeStartElement("Fonts");
        stream.writeStartElement("Node");
         writeFontToXML(&stream, scene->config()->_nodeFont);
        stream.writeEndElement();              // Node
        stream.writeStartElement("Edge");
         writeFontToXML(&stream, scene->config()->_edgeFont);
        stream.writeEndElement();              // Edge
       stream.writeEndElement();        // Fonts
      stream.writeEndElement();     // Design

      stream.writeStartElement("NodeList");
        for (auto& pair : scene->graph()->nodes())
        {
            stream.writeStartElement("Node");
            stream.writeAttribute("id", QString::number(pair.first));
            stream.writeAttribute("x", QString::number(pair.second->pos().x()));
            stream.writeAttribute("y", QString::number(pair.second->pos().y()));
            stream.writeAttribute("weight", QString::number(pair.second->weight()));
            stream.writeEndElement();     // Node
        }
      stream.writeEndElement();     // NodeList

      stream.writeStartElement("EdgeList");
        for (auto edge : scene->graph()->edges())
        {
            stream.writeStartElement("Edge");
            stream.writeAttribute("weight", QString::number(edge->weight()));
            stream.writeAttribute("from_id", QString::number(edge->fromNode()->id()));
            stream.writeAttribute("to_id", QString::number(edge->toNode()->id()));
            stream.writeEndElement();     // Edge
        }
      stream.writeEndElement();     // EdgeList
     stream.writeEndElement();  // Graph
    stream.writeEndDocument();
}

bool FileGraphManager::load(QFile &file, std::shared_ptr<WeightedGraph> &graph, std::shared_ptr<GraphConfiguration> &config)
{
    QDomDocument document;
    if (!document.setContent(&file)) return false;
    QDomElement root_element = document.documentElement();
    if (root_element.isNull() || root_element.nodeName() != "Graph") return false;
    QString isDirected = root_element.attribute("isDirected");
    if (isDirected.isEmpty()) return false;

    graph = (!isDirected.compare("true")) ?
                static_cast<std::shared_ptr<WeightedGraph>>(std::make_shared<DirectedGraph<NodeInfo, EdgeInfo>>()) :
                static_cast<std::shared_ptr<WeightedGraph>>(std::make_shared<UndirectedGraph<NodeInfo, EdgeInfo>>());
    config = std::make_shared<GraphConfiguration>();

    GraphParser parser;
    return parser.parse(root_element, graph, config);
}

void FileGraphManager::writePenToXML(QXmlStreamWriter *stream, const QPen &pen)
{
    stream->writeAttribute("penStyle", QString::number(pen.style()));
    stream->writeAttribute("capStyle", QString::number(pen.capStyle()));
    stream->writeAttribute("joinStyle", QString::number(pen.joinStyle()));
    stream->writeAttribute("width", QString::number(pen.widthF()));
    stream->writeAttribute("color", QString::number(pen.color().rgba()));
}

void FileGraphManager::writeBrushToXML(QXmlStreamWriter *stream, const QBrush &brush)
{
    stream->writeAttribute("brushStyle", QString::number(brush.style()));
    stream->writeAttribute("color", QString::number(brush.color().rgba()));
}

void FileGraphManager::writeFontToXML(QXmlStreamWriter *stream, const QFont &font)
{
    stream->writeAttribute("family", font.family());
    stream->writeAttribute("weight", QString::number(font.weight()));
    stream->writeAttribute("pointSize", QString::number(font.pointSize()));
}

bool FileGraphManager::XMLParser::parseElementByMap(QDomElement &element, std::map<QString, std::unique_ptr<FileGraphManager::XMLParser>> &parsers,
                                                    std::shared_ptr<WeightedGraph> &graph, std::shared_ptr<GraphConfiguration> &config)
{
    QDomElement child_element = element.firstChildElement();
    while (!parsers.empty()) {
        if (child_element.isNull()) return false;
        auto iter = parsers.find(child_element.nodeName());
        if (iter == parsers.end()) return false;
        if (!iter->second->parse(child_element, graph, config)) return false;
        parsers.erase(iter);
        child_element = child_element.nextSiblingElement();
    }
    return true;
}

bool FileGraphManager::XMLParser::setPenFromAttribute(QDomElement &element, QPen *pen)
{
    bool ok;
    pen->setStyle(static_cast<Qt::PenStyle>(element.attribute("penStyle").toInt(&ok)));
    if (!ok) return false;

    pen->setCapStyle(static_cast<Qt::PenCapStyle>(element.attribute("capStyle").toInt(&ok)));
    if (!ok) return false;

    pen->setJoinStyle(static_cast<Qt::PenJoinStyle>(element.attribute("joinStyle").toInt(&ok)));
    if (!ok) return false;

    pen->setWidthF(element.attribute("width").toDouble(&ok));
    if (!ok) return false;

    QRgb rgb = element.attribute("color").toUInt(&ok);
    QColor color(rgb);
    color.setAlpha(rgb >> 24);
    pen->setColor(color);
    if (!ok) return false;
    return true;
}

bool FileGraphManager::XMLParser::setBrushFromAttribute(QDomElement &element, QBrush *brush)
{
    bool ok;
    brush->setStyle(static_cast<Qt::BrushStyle>(element.attribute("brushStyle").toInt(&ok)));
    if (!ok) return false;

    QRgb rgb = element.attribute("color").toUInt(&ok);
    QColor color(rgb);
    color.setAlpha(rgb >> 24);
    brush->setColor(color);
    if (!ok) return false;
    return true;
}

bool FileGraphManager::XMLParser::setFontFromAttribute(QDomElement &element, QFont *font)
{
    font->setFamily(element.attribute("family"));

    bool ok;
    font->setWeight(element.attribute("weight").toInt(&ok));
    if (!ok) return false;

    font->setPointSize(element.attribute("pointSize").toInt(&ok));
    if (!ok) return false;
    return true;
}

bool FileGraphManager::GraphParser::parse(QDomElement &element, std::shared_ptr<WeightedGraph> &graph, std::shared_ptr<GraphConfiguration> &config) const {
    std::map<QString, std::unique_ptr<XMLParser>> sub_parsers;
    sub_parsers.emplace("Design", std::unique_ptr<XMLParser>(new DesignParser()));
    sub_parsers.emplace("NodeList", std::unique_ptr<XMLParser>(new NodeListParser()));
    sub_parsers.emplace("EdgeList", std::unique_ptr<XMLParser>(new EdgeListParser()));
    return parseElementByMap(element, sub_parsers, graph, config);
}

bool FileGraphManager::DesignParser::parse(QDomElement &element, std::shared_ptr<WeightedGraph> &graph, std::shared_ptr<GraphConfiguration> &config) const {
    std::map<QString, std::unique_ptr<XMLParser>> sub_parsers;
    sub_parsers.emplace("Types", std::unique_ptr<XMLParser>(new TypeParser()));
    sub_parsers.emplace("Pens", std::unique_ptr<XMLParser>(new PenParser()));
    sub_parsers.emplace("Brushes", std::unique_ptr<XMLParser>(new BrushParser()));
    sub_parsers.emplace("Fonts", std::unique_ptr<XMLParser>(new FontParser()));
    return parseElementByMap(element, sub_parsers, graph, config);
}

bool FileGraphManager::TypeParser::parse(QDomElement &element, std::shared_ptr<WeightedGraph> &graph, std::shared_ptr<GraphConfiguration> &config) const {
    Q_UNUSED(graph);
    bool ok;

    QString nodeType = element.attribute("node");
    int type = nodeType.toInt(&ok);
    if (!ok) return false;
    switch (type) {
        case QGraphicsEllipseNode::Type :
            config->_nodeCreator = std::unique_ptr<GraphicsNodeCreator>(new GraphicsNodeCreatorT<QGraphicsEllipseNode>());
            break;
        case QGraphicsRoundedRectNode::Type :
            config->_nodeCreator = std::unique_ptr<GraphicsNodeCreator>(new GraphicsNodeCreatorT<QGraphicsRoundedRectNode>());
            break;
        default:
            return false;
    }

    QString edgeType = element.attribute("edge");
    type = edgeType.toInt(&ok);
    if (!ok) return false;
    switch (type) {
        case QGraphicsSimpleLineEdge::Type :
            config->_edgeCreator = std::unique_ptr<GraphicsEdgeCreator>(new GraphicsEdgeCreatorT<QGraphicsSimpleLineEdge>());
            break;
        case QGraphicsCubicArrowEdge::Type :
            config->_edgeCreator = std::unique_ptr<GraphicsEdgeCreator>(new GraphicsEdgeCreatorT<QGraphicsCubicArrowEdge>());
            break;
        default: return false;
    }

    return true;
}

bool FileGraphManager::PenParser::parse(QDomElement &element, std::shared_ptr<WeightedGraph> &graph, std::shared_ptr<GraphConfiguration> &config) const {
    std::map<QString, std::unique_ptr<XMLParser>> sub_parsers;
    sub_parsers.emplace("Simple", std::unique_ptr<XMLParser>(new SimplePenParser()));
    sub_parsers.emplace("Hover", std::unique_ptr<XMLParser>(new HoverPenParser()));
    return parseElementByMap(element, sub_parsers, graph, config);
}

bool FileGraphManager::SimplePenParser::parse(QDomElement &element, std::shared_ptr<WeightedGraph> &graph, std::shared_ptr<GraphConfiguration> &config) const {
    Q_UNUSED(graph);
    std::map<QString, QPen*> pens;
    pens.emplace("Node", &config->_nodePen);
    pens.emplace("Edge", &config->_edgePen);

    QDomElement child_element = element.firstChildElement();
    while (!pens.empty()) {
        if (child_element.isNull()) return false;
        auto iter = pens.find(child_element.nodeName());
        if (iter == pens.end()) return false;
        if (!setPenFromAttribute(child_element, iter->second)) return false;
        pens.erase(iter);
        child_element = child_element.nextSiblingElement();
    }
    return true;
}

bool FileGraphManager::HoverPenParser::parse(QDomElement &element, std::shared_ptr<WeightedGraph> &graph, std::shared_ptr<GraphConfiguration> &config) const {
    Q_UNUSED(graph);
    std::map<QString, QPen*> pens;
    pens.emplace("Node", &config->_nodeHoverPen);
    pens.emplace("Edge", &config->_edgeHoverPen);

    QDomElement child_element = element.firstChildElement();
    while (!pens.empty()) {
        if (child_element.isNull()) return false;
        auto iter = pens.find(child_element.nodeName());
        if (iter == pens.end()) return false;
        if (!setPenFromAttribute(child_element, iter->second)) return false;
        pens.erase(iter);
        child_element = child_element.nextSiblingElement();
    }
    return true;
}

bool FileGraphManager::BrushParser::parse(QDomElement &element, std::shared_ptr<WeightedGraph> &graph, std::shared_ptr<GraphConfiguration> &config) const {
    Q_UNUSED(graph);
    std::map<QString, QBrush*> brushes;
    brushes.emplace("Node", &config->_nodeBrush);
    brushes.emplace("Edge", &config->_edgeBrush);

    QDomElement child_element = element.firstChildElement();
    while (!brushes.empty()) {
        if (child_element.isNull()) return false;
        auto iter = brushes.find(child_element.nodeName());
        if (iter == brushes.end()) return false;
        if (!setBrushFromAttribute(child_element, iter->second)) return false;
        brushes.erase(iter);
        child_element = child_element.nextSiblingElement();
    }
    return true;
}

bool FileGraphManager::FontParser::parse(QDomElement &element, std::shared_ptr<WeightedGraph> &graph, std::shared_ptr<GraphConfiguration> &config) const {
    Q_UNUSED(graph);
    std::map<QString, QFont*> fonts;
    fonts.emplace("Node", &config->_nodeFont);
    fonts.emplace("Edge", &config->_edgeFont);

    QDomElement child_element = element.firstChildElement();
    while (!fonts.empty()) {
        if (child_element.isNull()) return false;
        auto iter = fonts.find(child_element.nodeName());
        if (iter == fonts.end()) return false;
        if (!setFontFromAttribute(child_element, iter->second)) return false;
        fonts.erase(iter);
        child_element = child_element.nextSiblingElement();
    }
    return true;
}

bool FileGraphManager::NodeListParser::parse(QDomElement &element, std::shared_ptr<WeightedGraph> &graph, std::shared_ptr<GraphConfiguration> &config) const {
    Q_UNUSED(graph);
    Q_UNUSED(config);

    bool ok;
    QDomElement child_element = element.firstChildElement();
    while (!child_element.isNull()) {
        if (child_element.nodeName() != "Node") return false;
        WeightedNode *node = graph->createNode(child_element.attribute("id").toInt(&ok));
        if (!ok) return false;
        double x = child_element.attribute("x").toDouble(&ok);
        if (!ok) return false;
        double y = child_element.attribute("y").toDouble(&ok);
        if (!ok) return false;
        node->setPos(QPointF(x, y));
        node->setWeight(child_element.attribute("weight").toInt(&ok));
        if (!ok) return false;
        child_element = child_element.nextSiblingElement();
    }
    return true;
}

bool FileGraphManager::EdgeListParser::parse(QDomElement &element, std::shared_ptr<WeightedGraph> &graph, std::shared_ptr<GraphConfiguration> &config) const {
    Q_UNUSED(graph);
    Q_UNUSED(config);

    bool ok;
    QDomElement child_element = element.firstChildElement();
    while (!child_element.isNull()) {
        if (child_element.nodeName() != "Edge") return false;
        WeightedEdge *edge = graph->createEdge();
        edge->setWeight(child_element.attribute("weight").toInt(&ok));
        if (!ok) return false;
        auto iter = graph->nodes().find(child_element.attribute("from_id").toInt(&ok));
        if (!ok || iter == graph->nodes().end()) return false;
        WeightedNode *from_node = iter->second;
        iter = graph->nodes().find(child_element.attribute("to_id").toInt(&ok));
        if (!ok || iter == graph->nodes().end()) return false;
        WeightedNode *to_node = iter->second;
        edge->setNodes(from_node, to_node);
        child_element = child_element.nextSiblingElement();
    }
    return true;
}
