#ifndef BRUSHOBSERVERFRAME_H
#define BRUSHOBSERVERFRAME_H

#include <QFrame>

class BrushObserverFrame : public QFrame
{
public:
    BrushObserverFrame(QWidget *parent = 0);
    ~BrushObserverFrame();

    void setBrush(const QBrush &brush);
    QBrush brush() const;

    void setColor(const QColor &color);
    void setStyle(Qt::BrushStyle style);

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    QBrush _brush;
};

#endif // BRUSHOBSERVERFRAME_H
