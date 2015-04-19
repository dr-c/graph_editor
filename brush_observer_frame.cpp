#include "brush_observer_frame.h"

#include <QPainter>
#include <QColorDialog>

BrushObserverFrame::BrushObserverFrame(QWidget *parent)
    : QFrame(parent)
{

}

BrushObserverFrame::~BrushObserverFrame()
{

}

void BrushObserverFrame::setBrush(const QBrush &brush)
{
    _brush = brush;
    repaint();
}

QBrush BrushObserverFrame::brush() const
{
    return _brush;
}

void BrushObserverFrame::setColor(const QColor &color)
{
    _brush.setColor(color);
    repaint();
}

void BrushObserverFrame::setStyle(Qt::BrushStyle style)
{
    _brush.setStyle(style);
    repaint();
}

void BrushObserverFrame::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(frameRect(), _brush);
    QFrame::paintEvent(event);
}

void BrushObserverFrame::mouseReleaseEvent(QMouseEvent *event)
{
    QColorDialog dialog;
    dialog.setCurrentColor(_brush.color());
    dialog.setOption(QColorDialog::ShowAlphaChannel, true);
    if (dialog.exec() == QDialog::Accepted)
    {
        _brush.setColor(dialog.selectedColor());
        repaint();
    }
    QFrame::mouseReleaseEvent(event);
}

