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
    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void focusOutEvent(QFocusEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;

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
    Q_OBJECT

public:
    WeightEdgeTextItem(QGraphicsItem *parent = 0);
    WeightEdgeTextItem(const QString &text, QGraphicsItem *parent = 0);

    void calcCenterPoint(const QRectF &rect);
    void placeInCenter();
    QPointF center() const;

    void setBrush(const QBrush &brush);
    QBrush brush() const;

signals:
    void deleteKeyPressed();

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void focusOutEvent(QFocusEvent *event) override;

private:
    QPointF _center;
    QBrush  _backgroundBrush;
};

#endif // WEIGHTTEXTITEM_H
