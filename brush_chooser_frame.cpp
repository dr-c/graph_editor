#include "brush_chooser_frame.h"
#include "ui_brush_chooser_frame.h"

#include "brush_chooser_dialog.h"

#include <QPainter>

BrushChooserFrame::BrushChooserFrame(QWidget *parent)
    : QFrame(parent),
      _ui(new Ui::BrushChooserFrame)
{
    _ui->setupUi(this);
}

BrushChooserFrame::~BrushChooserFrame()
{
    delete _ui;
}

void BrushChooserFrame::setBrush(const QBrush &brush)
{
    _brush = brush;
    repaint();
}

QBrush BrushChooserFrame::brush() const
{
    return _brush;
}

void BrushChooserFrame::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(frameRect(), _brush);
    QFrame::paintEvent(event);
}

void BrushChooserFrame::mouseReleaseEvent(QMouseEvent *event)
{
    BrushChooserDialog dialog(this);
    dialog.setBrush(_brush);
    if (dialog.exec() == QDialog::Accepted)
    {
        _brush = dialog.brush();
        repaint();
    }
    QFrame::mouseReleaseEvent(event);
}
