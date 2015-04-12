#include "weight_text_item.h"

#include <QTextCursor>
#include <QTextDocument>
#include <QFocusEvent>
#include <QPainter>

#include <QDebug>

WeightTextItem::WeightTextItem(QGraphicsItem *parent)
    : QGraphicsTextItem(parent)
{
    init();
}

WeightTextItem::WeightTextItem(const int num, QGraphicsItem *parent)
    : QGraphicsTextItem(QString::number(num), parent)
{
    if (num < 0 || num > 999)
        setPlainText(QString::number(_defaultWeight));

    init();
}

WeightTextItem::WeightTextItem(const QString &text, QGraphicsItem *parent)
    : QGraphicsTextItem(text, parent)
{
    bool ok;
    int num = text.toInt(&ok);
    if (num < 0 || num > 999 || !ok)
        setPlainText(QString::number(_defaultWeight));

    init();
}

int WeightTextItem::type() const
{
    return Type;
}

void WeightTextItem::validateContentsChanging()
{
    QString content = toPlainText();

    if (content == _prevText)
        return;

    _prevCursorPosition = textCursor().position();

    while (!content.isEmpty() && content.at(0) == '0')
    {
        content = content.right(content.length() - 1);
        if (_prevCursorPosition > 0)
            _prevCursorPosition--;
    }
    if (content.isEmpty())
    {
        _prevText = content;
        setPlainText(_prevText);
        emit textChanged(_defaultWeight);
        return;
    }

    if (content.length() > 3)
    {
        content = content.left(3);
        if (_prevCursorPosition > 3)
            _prevCursorPosition = 3;
    }

    bool ok;
    content.toInt(&ok);
    if (ok)
        _prevText = content;

    setPlainText(_prevText);

    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor, _prevCursorPosition);
    setTextCursor(cursor);

    emit textChanged(_prevText.toInt());
}

void WeightTextItem::focusInEvent(QFocusEvent *event)
{
    if (textInteractionFlags() == Qt::TextEditorInteraction)
    {
        _firstText = toPlainText();
        _prevText = _firstText;
    }
    QGraphicsTextItem::focusInEvent(event);
}

void WeightTextItem::focusOutEvent(QFocusEvent *event)
{
    if (textInteractionFlags() == Qt::TextEditorInteraction)
    {
        if (toPlainText().isEmpty())
            setPlainText(_firstText);
        QTextCursor cursor = textCursor();
        cursor.clearSelection();
        setTextCursor(cursor);
        QGraphicsTextItem::focusOutEvent(event);
        setTextInteractionFlags(Qt::NoTextInteraction);
    }
    else
        QGraphicsTextItem::focusOutEvent(event);
}

void WeightTextItem::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Escape:
        setPlainText(_firstText);
        clearFocus();
        break;
    case Qt::Key_Return:
        clearFocus();
        break;
    default:
        QGraphicsTextItem::keyPressEvent(event);
    }
}

void WeightTextItem::init()
{
    connect(document(), SIGNAL(contentsChanged()), this, SLOT(validateContentsChanging()));
}

WeightEdgeTextItem::WeightEdgeTextItem(QGraphicsItem *parent)
    : WeightTextItem(parent), _backgroundBrush(QBrush(QColor(Qt::white)))
{
    setFlag(QGraphicsItem::ItemIsFocusable, true);
}

WeightEdgeTextItem::WeightEdgeTextItem(const QString &text, QGraphicsItem *parent)
    : WeightTextItem(text, parent)
{
    setFlag(QGraphicsItem::ItemIsFocusable, true);
}

int WeightEdgeTextItem::type() const
{
    return Type;
}

void WeightEdgeTextItem::calcCenterPoint(const QRectF &rect)
{
    _center = rect.center();
}

void WeightEdgeTextItem::setCenterPoint(const QPointF &point)
{
    _center = point;
}

void WeightEdgeTextItem::placeInCenter()
{
    const QSizeF &size = boundingRect().size();
    setPos(_center.x() - size.width() / 2, _center.y() - size.height() / 2);
}

const QPointF &WeightEdgeTextItem::center() const
{
    return _center;
}

void WeightEdgeTextItem::setBrush(const QBrush &brush)
{
    _backgroundBrush = brush;
}

const QBrush &WeightEdgeTextItem::brush() const
{
    return _backgroundBrush;
}

void WeightEdgeTextItem::keyPressEvent(QKeyEvent *event)
{
    QGraphicsTextItem::keyPressEvent(event);
    if (event->key() == Qt::Key_Delete && textInteractionFlags() != Qt::TextEditorInteraction)
        emit deleteKeyPressed();
}

void WeightEdgeTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(_backgroundBrush);
    painter->drawRect(boundingRect());
    WeightTextItem::paint(painter, option, widget);
}

void WeightEdgeTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    WeightTextItem::mouseDoubleClickEvent(event);
    clearFocus();
    setTextInteractionFlags(Qt::TextEditorInteraction);
    setFocus();
}

void WeightEdgeTextItem::focusOutEvent(QFocusEvent *event)
{
    WeightTextItem::focusOutEvent(event);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
}

