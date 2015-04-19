#include "pen_observer_frame.h"

#include <QPainter>
#include <QColorDialog>

PenObserverFrame::PenObserverFrame(QWidget *parent)
    : QFrame(parent)
{

}

PenObserverFrame::~PenObserverFrame()
{

}

void PenObserverFrame::setPen(const QPen &pen)
{
    _pen = pen;
    repaint();
}

const QPen &PenObserverFrame::pen() const
{
    return _pen;
}

void PenObserverFrame::setColor(const QColor &color)
{
    _pen.setColor(color);
    repaint();
}

void PenObserverFrame::setWidth(double width)
{
    _pen.setWidthF(width);
    repaint();
}

void PenObserverFrame::setCapStyle(Qt::PenCapStyle style)
{
    _pen.setCapStyle(style);
    repaint();
}

void PenObserverFrame::setJoinStyle(Qt::PenJoinStyle style)
{
    _pen.setJoinStyle(style);
    repaint();
}

void PenObserverFrame::setStyle(Qt::PenStyle style)
{
    _pen.setStyle(style);
    repaint();
}


void PenObserverFrame::paintEvent(QPaintEvent *event)
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

void PenObserverFrame::mouseReleaseEvent(QMouseEvent *event)
{
    QColorDialog dialog;
    dialog.setOption(QColorDialog::ShowAlphaChannel, true);
    dialog.setCurrentColor(_pen.color());
    if (dialog.exec() == QDialog::Accepted)
    {
        _pen.setColor(dialog.selectedColor());
        repaint();
    }
    QFrame::mouseReleaseEvent(event);
}
