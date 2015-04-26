#include "brush_chooser_dialog.h"
#include "ui_brush_chooser_dialog.h"

#include <QPushButton>

const std::array<QString, BrushChooserDialog::array_size> BrushChooserDialog::names = {{
    "NoBrush", "SolidPattern", "Dense1Pattern",
    "Dense2Pattern", "Dense3Pattern", "Dense4Pattern",
    "Dense5Pattern", "Dense6Pattern", "Dense7Pattern",
    "HorPattern", "VerPattern", "CrossPattern",
    "BDiagPattern", "FDiagPattern", "DiagCrossPattern"
}};
const std::array<Qt::BrushStyle, BrushChooserDialog::array_size> BrushChooserDialog::styles = {{
     Qt::NoBrush, Qt::SolidPattern, Qt::Dense1Pattern,
     Qt::Dense2Pattern, Qt::Dense3Pattern, Qt::Dense4Pattern,
     Qt::Dense5Pattern, Qt::Dense6Pattern, Qt::Dense7Pattern,
     Qt::HorPattern, Qt::VerPattern, Qt::CrossPattern,
     Qt::BDiagPattern, Qt::FDiagPattern, Qt::DiagCrossPattern
}};

BrushChooserDialog::BrushChooserDialog(QWidget *parent)
    : QDialog(parent),
      _ui(new Ui::BrushChooserDialog),
      _styleButtonGroup(this)
{
    _ui->setupUi(this);

    connect(&_styleButtonGroup, SIGNAL(buttonToggled(int,bool)), this, SLOT(setStyle(int,bool)));

    for (int i = 0; i < array_size; i++)
    {
        QPushButton *button = new QPushButton(names[i], _ui->styleGroupBox);
        button->setCheckable(true);
        _ui->gridLayout->addWidget(button, i / 3, i % 3, 1, 1);
        _styleButtonGroup.addButton(button, styles[i]);
    }
}

BrushChooserDialog::~BrushChooserDialog()
{
    for (auto button : _styleButtonGroup.buttons())
        delete button;
    delete _ui;
}

void BrushChooserDialog::setBrush(const QBrush &brush)
{
    _ui->frame->setBrush(brush);
    _styleButtonGroup.button(brush.style())->click();
}

QBrush BrushChooserDialog::brush() const
{
    return _ui->frame->brush();
}

void BrushChooserDialog::setStyle(int id, bool checked)
{
    if (checked)
        _ui->frame->setStyle(static_cast<Qt::BrushStyle>(id));
}
