#include "graph_creation_dialog.h"
#include "ui_graph_creation_dialog.h"

#include <QFontDialog>
#include <QButtonGroup>

#include "graph_scene.h"

GraphCreationDialog::GraphCreationDialog(QWidget *parent)
    : QDialog(parent),
      _ui(new Ui::GraphCreationDialog),
      _graphTypeGroup(new QButtonGroup(this)),
      _shapeNodesGroup(new QButtonGroup(this)),
      _formEdgesGroup(new QButtonGroup(this))
{
    _ui->setupUi(this);

    init();

    _graphTypeGroup->addButton(_ui->directedGraphRadioButton);
    _graphTypeGroup->addButton(_ui->undirectedGraphRadioButton);
    _shapeNodesGroup->addButton(_ui->circleButton);
    _shapeNodesGroup->addButton(_ui->rectangleButton);
    _formEdgesGroup->addButton(_ui->arrowButton);
    _formEdgesGroup->addButton(_ui->simpleLineButton);

    connect(_ui->resetButton, SIGNAL(clicked()), this, SLOT(init()));
}

GraphCreationDialog::~GraphCreationDialog()
{
    delete _formEdgesGroup;
    delete _shapeNodesGroup;
    delete _graphTypeGroup;
    delete _ui;
}

bool GraphCreationDialog::isDirected() const
{
    return _ui->directedGraphRadioButton->isChecked();
}

void GraphCreationDialog::configureGraphScene(BasicGraphScene *scene) const
{
    scene->setNodePen(_ui->nodePenFrame->pen());
    scene->setNodeHoverPen(_ui->nodeHoverPenFrame->pen());
    scene->setNodeBrush(_ui->nodeBrushFrame->brush());
    scene->setNodeFont(_ui->nodeFontButton->font());
    scene->setEdgePen(_ui->edgePenFrame->pen());
    scene->setEdgeHoverPen(_ui->edgeHoverPenFrame->pen());
    scene->setEdgeBrush(_ui->edgeBrushFrame->brush());
    scene->setEdgeFont(_ui->edgeFontButton->font());
}

void GraphCreationDialog::on_nodeFontButton_clicked()
{
    changeFont(_ui->nodeFontButton);
}

void GraphCreationDialog::on_edgeFontButton_clicked()
{
    changeFont(_ui->edgeFontButton);
}

void GraphCreationDialog::init()
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
