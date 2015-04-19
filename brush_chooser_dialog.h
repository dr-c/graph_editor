#ifndef BRUSH_CHOOSER_DIALOG_H
#define BRUSH_CHOOSER_DIALOG_H

#include <QDialog>
#include <array>

namespace Ui {
class BrushChooserDialog;
}

class QButtonGroup;

class BrushChooserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BrushChooserDialog(QWidget *parent = 0);
    virtual ~BrushChooserDialog() override;

    void setBrush(const QBrush &brush);
    QBrush brush() const;

private slots:
    void setStyle(int id, bool checked);

private:
    Ui::BrushChooserDialog *_ui;
    QButtonGroup *_styleButtonGroup;

    static constexpr int array_size = 15;
    static const std::array<QString, array_size> names;
    static const std::array<Qt::BrushStyle, array_size> styles;
};

#endif // BRUSH_CHOOSER_DIALOG_H
