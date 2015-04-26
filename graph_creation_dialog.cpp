#include "graph_creation_dialog.h"
#include "ui_graph_creation_dialog.h"

#include <QFontDialog>

#include "graph_scene.h"
#include "graph_configuration.h"

#include "qgraphics_ellipse_node.h"
#include "qgraphics_rounded_rect_node.h"
#include "qgraphics_cubic_arrow_edge.h"
#include "qgraphics_simple_line_edge.h"

GraphCreationDialog::GraphCreationDialog(QWidget *parent)
    : QDialog(parent),
      _ui(new Ui::GraphCreationDialog),
      _graphTypeGroup(this),
      _shapeNodesGroup(this),
      _formEdgesGroup(this)
{
    _ui->setupUi(this);

    reset();

    _graphTypeGroup.addButton(_ui->undirectedGraphRadioButton, 0);
    _graphTypeGroup.addButton(_ui->directedGraphRadioButton, 1);
    _shapeNodesGroup.addButton(_ui->circleButton, QGraphicsEllipseNode::Type);
    _shapeNodesGroup.addButton(_ui->rectangleButton, QGraphicsRoundedRectNode::Type);
    _formEdgesGroup.addButton(_ui->arrowButton, QGraphicsCubicArrowEdge::Type);
    _formEdgesGroup.addButton(_ui->simpleLineButton, QGraphicsSimpleLineEdge::Type);

    connect(_ui->resetButton, SIGNAL(clicked()), this, SLOT(reset()));
}

GraphCreationDialog::~GraphCreationDialog()
{
    delete _ui;
}

void GraphCreationDialog::setDirected(bool isDirected)
{
    _graphTypeGroup.button(static_cast<int>(isDirected))->setChecked(true);
}

bool GraphCreationDialog::isDirected() const
{
    return static_cast<bool>(_graphTypeGroup.checkedId());
}

void GraphCreationDialog::lockGraphTypeButtons()
{
    _ui->directedGraphRadioButton->setEnabled(false);
    _ui->undirectedGraphRadioButton->setEnabled(false);
}

void GraphCreationDialog::unLockGraphTypeButtons()
{
    _ui->directedGraphRadioButton->setEnabled(true);
    _ui->undirectedGraphRadioButton->setEnabled(true);
}

void GraphCreationDialog::setConfiguration(const std::shared_ptr<GraphConfiguration> &config)
{
    _shapeNodesGroup.button(config->_nodeCreator->type())->setChecked(true);
    _formEdgesGroup.button(config->_edgeCreator->type())->setChecked(true);
    _ui->nodePenFrame->setPen(config->_nodePen);
    _ui->nodeHoverPenFrame->setPen(config->_nodeHoverPen);
    _ui->nodeBrushFrame->setBrush(config->_nodeBrush);
    _ui->nodeFontButton->setFont(config->_nodeFont);
    _ui->edgePenFrame->setPen(config->_edgePen);
    _ui->edgeHoverPenFrame->setPen(config->_edgeHoverPen);
    _ui->edgeBrushFrame->setBrush(config->_edgeBrush);
    _ui->edgeFontButton->setFont(config->_edgeFont);
}

std::shared_ptr<GraphConfiguration> GraphCreationDialog::getConfiguration() const
{
    auto config = std::make_shared<GraphConfiguration>();
    config->_nodeCreator = std::unique_ptr<GraphicsNodeCreator>(getNodeCreator());
    config->_edgeCreator = std::unique_ptr<GraphicsEdgeCreator>(getEdgeCreator());
    config->_nodePen = _ui->nodePenFrame->pen();
    config->_nodeHoverPen = _ui->nodeHoverPenFrame->pen();
    config->_nodeBrush = _ui->nodeBrushFrame->brush();
    config->_nodeFont = _ui->nodeFontButton->font();
    config->_edgePen = _ui->edgePenFrame->pen();
    config->_edgeHoverPen = _ui->edgeHoverPenFrame->pen();
    config->_edgeBrush = _ui->edgeBrushFrame->brush();
    config->_edgeFont = _ui->edgeFontButton->font();
    return config;
}

void GraphCreationDialog::on_nodeFontButton_clicked()
{
    changeFont(_ui->nodeFontButton);
}

void GraphCreationDialog::on_edgeFontButton_clicked()
{
    changeFont(_ui->edgeFontButton);
}

void GraphCreationDialog::reset()
{
    _ui->nodePenFrame->setPen(QPen(QColor(Qt::blue), 2));
    _ui->nodeHoverPenFrame->setPen(QPen(QColor(Qt::red), 2));
    _ui->nodeBrushFrame->setBrush(QBrush(QColor(192, 192, 192, 192)));
    _ui->nodeFontButton->setFont(QFont("Times New Roman", 10));
    _ui->edgePenFrame->setPen(QPen(QColor(Qt::black), 2)),
    _ui->edgeHoverPenFrame->setPen(QPen(QColor(Qt::red), 2)),
    _ui->edgeBrushFrame->setBrush(QBrush(QColor(192, 192, 192, 255))),
    _ui->edgeFontButton->setFont(QFont("Times New Roman", 10));
}

void GraphCreationDialog::changeFont(QPushButton *button)
{
    QFontDialog dialog;
    dialog.setCurrentFont(button->font());
    if (dialog.exec() == QDialog::Accepted)
    {
        QFont font = dialog.selectedFont();
        if (font.pointSize() > 14)
            font.setPointSize(14);
        button->setFont(font);
    }
    button->clearFocus();
}

GraphicsNodeCreator *GraphCreationDialog::getNodeCreator() const
{
    switch (_shapeNodesGroup.checkedId()) {
        case QGraphicsEllipseNode::Type :       return new GraphicsNodeCreatorT<QGraphicsEllipseNode>();
        case QGraphicsRoundedRectNode::Type :   return new GraphicsNodeCreatorT<QGraphicsRoundedRectNode>();
        default: assert(false);
    }
}

GraphicsEdgeCreator *GraphCreationDialog::getEdgeCreator() const
{
    switch (_formEdgesGroup.checkedId()) {
        case QGraphicsCubicArrowEdge::Type :    return new GraphicsEdgeCreatorT<QGraphicsCubicArrowEdge>();
        case QGraphicsSimpleLineEdge::Type :    return new GraphicsEdgeCreatorT<QGraphicsSimpleLineEdge>();
        default: assert(false);
    }
}
