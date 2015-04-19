#ifndef PEN_CHOOSER_FRAME_H
#define PEN_CHOOSER_FRAME_H

#include <QFrame>
#include <QPen>

namespace Ui {
class PenChooserFrame;
}

class PenChooserFrame : public QFrame
{
    Q_OBJECT

public:
    explicit PenChooserFrame(QWidget *parent = 0);
    ~PenChooserFrame();

    void setPen(const QPen &pen);
    QPen pen() const;

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::PenChooserFrame *_ui;
    QPen _pen;
};

#endif // PEN_CHOOSER_FRAME_H
