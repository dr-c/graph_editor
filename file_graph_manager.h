#ifndef FILEGRAPHMANAGER_H
#define FILEGRAPHMANAGER_H

#include <memory>
#include "item_info.h"

class QFile;
class GraphScene;
class QXmlStreamWriter;
class QDomElement;
class QPen;
class QBrush;
class QFont;
class GraphConfiguration;

class FileGraphManager
{
public:
    static void save(QFile &file, GraphScene *scene);
    static bool load(QFile &file, std::shared_ptr<WeightedGraph> &graph, std::shared_ptr<GraphConfiguration> &config);
private:
    FileGraphManager() = delete;

    static void writePenToXML(QXmlStreamWriter *stream, const QPen &pen);
    static void writeBrushToXML(QXmlStreamWriter *stream, const QBrush &brush);
    static void writeFontToXML(QXmlStreamWriter *stream, const QFont &font);

    struct XMLParser {
        virtual bool parse(QDomElement &element, std::shared_ptr<WeightedGraph> &graph, std::shared_ptr<GraphConfiguration> &config) const = 0;
    protected:
        static bool parseElementByMap(QDomElement &element, std::map<QString, std::unique_ptr<XMLParser>> &parsers,
                                      std::shared_ptr<WeightedGraph> &graph, std::shared_ptr<GraphConfiguration> &config);
        static bool setPenFromAttribute(QDomElement &element, QPen* pen);
        static bool setBrushFromAttribute(QDomElement &element, QBrush* brush);
        static bool setFontFromAttribute(QDomElement &element, QFont* font);
    };
    struct GraphParser : public XMLParser {
        virtual bool parse(QDomElement &element, std::shared_ptr<WeightedGraph> &graph, std::shared_ptr<GraphConfiguration> &config) const override;
    };
    struct DesignParser : public XMLParser {
        virtual bool parse(QDomElement &element, std::shared_ptr<WeightedGraph> &graph, std::shared_ptr<GraphConfiguration> &config) const override;
    };
    struct TypeParser : public XMLParser {
        virtual bool parse(QDomElement &element, std::shared_ptr<WeightedGraph> &graph, std::shared_ptr<GraphConfiguration> &config) const override;
    };
    struct PenParser : public XMLParser {
        virtual bool parse(QDomElement &element, std::shared_ptr<WeightedGraph> &graph, std::shared_ptr<GraphConfiguration> &config) const override;
    };
    struct SimplePenParser : public XMLParser {
        virtual bool parse(QDomElement &element, std::shared_ptr<WeightedGraph> &graph, std::shared_ptr<GraphConfiguration> &config) const override;
    };
    struct HoverPenParser : public XMLParser {
        virtual bool parse(QDomElement &element, std::shared_ptr<WeightedGraph> &graph, std::shared_ptr<GraphConfiguration> &config) const override;
    };
    struct BrushParser : public XMLParser {
        virtual bool parse(QDomElement &element, std::shared_ptr<WeightedGraph> &graph, std::shared_ptr<GraphConfiguration> &config) const override;
    };
    struct FontParser : public XMLParser {
        virtual bool parse(QDomElement &element, std::shared_ptr<WeightedGraph> &graph, std::shared_ptr<GraphConfiguration> &config) const override;
    };
    struct NodeListParser : public XMLParser {
        virtual bool parse(QDomElement &element, std::shared_ptr<WeightedGraph> &graph, std::shared_ptr<GraphConfiguration> &config) const override;
    };
    struct EdgeListParser : public XMLParser {
        virtual bool parse(QDomElement &element, std::shared_ptr<WeightedGraph> &graph, std::shared_ptr<GraphConfiguration> &config) const override;
    };
};

#endif // FILEGRAPHMANAGER_H
