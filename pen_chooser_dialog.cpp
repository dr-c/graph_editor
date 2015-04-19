#include "pen_chooser_dialog.h"
#include "ui_pen_chooser_dialog.h"

#include <QButtonGroup>

const std::array<QString, PenChooserDialog::pen_array_size> PenChooserDialog::pen_names = {{
    "NoPen", "SolidLine", "DashLine",
    "DotLine", "DashDotLine", "DashDotDotLine"
}};
const std::array<Qt::PenStyle, PenChooserDialog::pen_array_size> PenChooserDialog::pen_styles = {{
     Qt::NoPen, Qt::SolidLine, Qt::DashLine,
     Qt::DotLine, Qt::DashDotLine, Qt::DashDotDotLine
}};

const std::array<QString, PenChooserDialog::cap_array_size> PenChooserDialog::cap_names = {{
    "FlatCap", "SquareCap", "RoundCap"
}};
const std::array<Qt::PenCapStyle, PenChooserDialog::cap_array_size> PenChooserDialog::cap_styles = {{
     Qt::FlatCap, Qt::SquareCap, Qt::RoundCap
}};

const std::array<QString, PenChooserDialog::join_array_size> PenChooserDialog::join_names = {{
    "MiterJoin", "BevelJoin", "RoundJoin"
}};
const std::array<Qt::PenJoinStyle, PenChooserDialog::join_array_size> PenChooserDialog::join_styles = {{
     Qt::MiterJoin, Qt::BevelJoin, Qt::RoundJoin
}};

template<typename Style, std::size_t sz>
void PenChooserDialog::initButtonGroup(const std::array<QString, sz> &names, const std::array<Style, sz> &styles,
                                       QGroupBox *box, QGridLayout *layout, QButtonGroup *group)
{
    for (std::size_t i = 0; i < sz; i++)
    {
        QPushButton *button = new QPushButton(names[i], box);
        button->setCheckable(true);
        layout->addWidget(button, i / 3, i % 3, 1, 1);
        group->addButton(button, styles[i]);
    }
}

PenChooserDialog::PenChooserDialog(QWidget *parent)
    : QDialog(parent),
      _ui(new Ui::PenChooserDialog),
      _penButtonGroup(new QButtonGroup(this)),
      _capButtonGroup(new QButtonGroup(this)),
      _joinButtonGroup(new QButtonGroup(this))
{
    _ui->setupUi(this);

    initButtonGroup<Qt::PenStyle, pen_array_size>(pen_names, pen_styles, _ui->penGroupBox, _ui->penGridLayout, _penButtonGroup);
    initButtonGroup<Qt::PenCapStyle, cap_array_size>(cap_names, cap_styles, _ui->capGroupBox, _ui->capGridLayout, _capButtonGroup);
    initButtonGroup<Qt::PenJoinStyle, join_array_size>(join_names, join_styles, _ui->joinGroupBox, _ui->joinGridLayout, _joinButtonGroup);

    connect(_ui->doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setWidth(double)));
    connect(_penButtonGroup, SIGNAL(buttonToggled(int,bool)), this, SLOT(setPenStyle(int,bool)));
    connect(_capButtonGroup, SIGNAL(buttonToggled(int,bool)), this, SLOT(setCapStyle(int,bool)));
    connect(_joinButtonGroup, SIGNAL(buttonToggled(int,bool)), this, SLOT(setJoinStyle(int,bool)));
}

PenChooserDialog::~PenChooserDialog()
{
    for (auto button : _joinButtonGroup->buttons())
        delete button;
    for (auto button : _capButtonGroup->buttons())
        delete button;
    for (auto button : _penButtonGroup->buttons())
        delete button;
    delete _joinButtonGroup;
    delete _capButtonGroup;
    delete _penButtonGroup;
    delete _ui;
}

void PenChooserDialog::setPen(const QPen &pen)
{
    _ui->frame->setPen(pen);
    _ui->doubleSpinBox->setValue(pen.widthF());
    _penButtonGroup->button(pen.style())->click();
    _capButtonGroup->button(pen.capStyle())->click();
    _joinButtonGroup->button(pen.joinStyle())->click();
}

QPen PenChooserDialog::pen() const
{
    return _ui->frame->pen();
}

void PenChooserDialog::setWidth(double width)
{
    _ui->frame->setWidth(width);
}

void PenChooserDialog::setPenStyle(int id, bool checked)
{
    if (checked)
        _ui->frame->setStyle(static_cast<Qt::PenStyle>(id));
}

void PenChooserDialog::setCapStyle(int id, bool checked)
{
    if (checked)
        _ui->frame->setCapStyle(static_cast<Qt::PenCapStyle>(id));
}

void PenChooserDialog::setJoinStyle(int id, bool checked)
{
    if (checked)
        _ui->frame->setJoinStyle(static_cast<Qt::PenJoinStyle>(id));
}
