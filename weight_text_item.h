#ifndef WEIGHTTEXTITEM_H
#define WEIGHTTEXTITEM_H

#include <QGraphicsTextItem>
#include <QBrush>

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

class WeightEdgeTextItem : public WeightTextItem
{
public:
    WeightEdgeTextItem(QGraphicsItem *parent = 0);
    WeightEdgeTextItem(const QString &text, QGraphicsItem *parent = 0);

    void calcCenterPoint(const QRectF &rect);
    void placeInCenter();
    QPointF center() const;

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
    QPointF _center;

    static const QBrush backgroundBrush;
};

#endif // WEIGHTTEXTITEM_H
