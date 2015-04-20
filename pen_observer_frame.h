/*
 * PenObserverFrame - Frame, which stores and displays Pen chosen by PenChooserDialog.
 *
 * OnMouseClick, it shows QColorDialog, which allows to change Color.
 */

#ifndef PENOBSERVERFRAME_H
#define PENOBSERVERFRAME_H

#include <QFrame>
#include <QPen>

class PenObserverFrame : public QFrame
{
public:
    PenObserverFrame(QWidget *parent);
    ~PenObserverFrame();

    void setPen(const QPen &pen);
    const QPen &pen() const;

    void setColor(const QColor &color);
    void setWidth(double width);
    void setCapStyle(Qt::PenCapStyle style);
    void setJoinStyle(Qt::PenJoinStyle style);
    void setStyle(Qt::PenStyle style);

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    QPen _pen;
};


#endif // PENOBSERVERFRAME_H
