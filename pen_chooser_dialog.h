/*
 * PenChooserDialog - Dialog, which allows to change Pen parameters: color and style.
 *
 * PenChooserDialog contains PenObserverFrame(uses for color control)
 *  and buttons(uses for style, capStyle and joinStyle control).
 * CustomDashLine style not supported.
 * More info about Pen styles on http://doc.qt.io/qt-5/qpen.html#details
 *
 * BrushChooserDialog used by BrushChooserFrame.
 */

#ifndef PEN_CHOOSER_DIALOG_H
#define PEN_CHOOSER_DIALOG_H

#include <QDialog>
#include <array>

class QGroupBox;
class QGridLayout;
class QButtonGroup;

namespace Ui {
class PenChooserDialog;
}

class PenChooserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PenChooserDialog(QWidget *parent = 0);
    ~PenChooserDialog();

    void setPen(const QPen &pen);
    QPen pen() const;

private slots:
    void setWidth(double width);
    void setPenStyle(int id, bool checked);
    void setCapStyle(int id, bool checked);
    void setJoinStyle(int id, bool checked);

private:
    Ui::PenChooserDialog *_ui;
    QButtonGroup *_penButtonGroup;
    QButtonGroup *_capButtonGroup;
    QButtonGroup *_joinButtonGroup;

    template<typename Style, std::size_t sz>
    void initButtonGroup(const std::array<QString, sz> &names, const std::array<Style, sz> &styles,
                         QGroupBox *box, QGridLayout *layout, QButtonGroup *group);

    static constexpr int pen_array_size = 6;
    static constexpr int cap_array_size = 3;
    static constexpr int join_array_size = 3;

    static const std::array<QString, pen_array_size> pen_names;
    static const std::array<QString, cap_array_size> cap_names;
    static const std::array<QString, join_array_size> join_names;

    static const std::array<Qt::PenStyle, pen_array_size> pen_styles;
    static const std::array<Qt::PenCapStyle, cap_array_size> cap_styles;
    static const std::array<Qt::PenJoinStyle, join_array_size> join_styles;
};

#endif // PEN_CHOOSER_DIALOG_H
