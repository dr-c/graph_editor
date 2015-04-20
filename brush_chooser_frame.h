/*
 * BrushChooserFrame - Frame, which stores and displays chosen Brush for different purposes.
 *
 * OnMouseClick, it shows BrushChooserDialog, which allows to change Brush.
 */

#ifndef BRUSH_CHOOSER_FRAME_H
#define BRUSH_CHOOSER_FRAME_H

#include <QFrame>

namespace Ui {
class BrushChooserFrame;
}

class BrushChooserFrame : public QFrame
{
    Q_OBJECT

public:
    explicit BrushChooserFrame(QWidget *parent = 0);
    ~BrushChooserFrame();

    void setBrush(const QBrush &brush);
    QBrush brush() const;

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::BrushChooserFrame *_ui;
    QBrush _brush;
};

#endif // BRUSH_CHOOSER_FRAME_H
