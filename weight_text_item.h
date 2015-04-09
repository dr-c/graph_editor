#ifndef WEIGHTTEXTITEM_H
#define WEIGHTTEXTITEM_H

#include <QGraphicsTextItem>

class WeightTextItem : public QGraphicsTextItem
{
    Q_OBJECT

public:
    enum { Type = UserType + 301 };

    WeightTextItem(QGraphicsItem *parent = 0);
    WeightTextItem(const int num, QGraphicsItem *parent = 0);
    WeightTextItem(const QString &text, QGraphicsItem *parent = 0);

    virtual int type() const;

protected:
    virtual void focusInEvent(QFocusEvent *event);
    virtual void focusOutEvent(QFocusEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

signals:
    void textChanged(int);

private slots:
    void validateContentsChanging();

private:
    void init();

    QString _firstText;
    QString _prevText;
    int     _prevCursorPosition;

    static const int _defaultWeight = 1;
};

#endif // WEIGHTTEXTITEM_H
