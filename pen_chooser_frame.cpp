#include "pen_chooser_frame.h"
#include "ui_pen_chooser_frame.h"

#include "pen_chooser_dialog.h"

#include <QPainter>
#include <QSize>

PenChooserFrame::PenChooserFrame(QWidget *parent)
    : QFrame(parent),
      _ui(new Ui::PenChooserFrame)
{
    _ui->setupUi(this);
}

PenChooserFrame::~PenChooserFrame()
{
    delete _ui;
}

void PenChooserFrame::setPen(const QPen &pen)
{
    _pen = pen;
}

QPen PenChooserFrame::pen() const
{
    return _pen;
}

void PenChooserFrame::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(_pen);
    QPainterPath path;
    QSize size = this->size();
    path.moveTo(5, size.height()/2);
    path.cubicTo(size.width()/2, 0, size.width()/2, size.height(), size.width() - 5, size.height()/2);
    painter.drawPath(path);
    QFrame::paintEvent(event);
}

void PenChooserFrame::mouseReleaseEvent(QMouseEvent *event)
{
    PenChooserDialog dialog(this);
    dialog.setPen(_pen);
    if (dialog.exec() == QDialog::Accepted)
    {
        _pen = dialog.pen();
        repaint();
    }
    QFrame::mouseReleaseEvent(event);
}
