/*
 * WeightTextItem - item for displaying weigth of QGraphicsNode.
 *
 * It can manipulate with positive integer numbers(until 3 digits).
 * When text changed, it emits textChanged(new_weight) signal.
 *
 * WeightEdgeTextItem - item for displaying weigth of QGraphicsEdge.
 * WeightEdgeTextItem is the same as WeightTextItem, but it has background for painting
 *  and can emit deleteKeyPressed() when text field has focus and key_delete pressed.
 */

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
    virtual ~WeightTextItem() override = default;

    virtual int type() const override;

protected:
    virtual void    focusInEvent(QFocusEvent *event) override;
    virtual void    focusOutEvent(QFocusEvent *event) override;
    virtual void    keyPressEvent(QKeyEvent *event) override;

signals:
    void textChanged(int);
    void finishTextChanging(int);

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
    Q_OBJECT

public:
    enum { Type = UserType + 302 };

    WeightEdgeTextItem(QGraphicsItem *parent = 0);
    WeightEdgeTextItem(const QString &text, QGraphicsItem *parent = 0);
    virtual ~WeightEdgeTextItem() override = default;

    virtual int type() const override;

    void calcCenterPoint(const QRectF &rect);
    void setCenterPoint(const QPointF &point);
    void placeInCenter();
    const QPointF &center() const;

    void setBrush(const QBrush &brush);
    const QBrush &brush() const;

signals:
    void deleteKeyPressed();
    void startTextChanging(int);

protected:
    virtual void    keyPressEvent(QKeyEvent *event) override;
    virtual void    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual void    mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void    focusOutEvent(QFocusEvent *event) override;

private:
    QPointF _center;
    QBrush  _backgroundBrush;
};

#endif // WEIGHTTEXTITEM_H
