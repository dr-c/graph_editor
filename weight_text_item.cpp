#include "weight_text_item.h"

#include <QTextCursor>
#include <QTextDocument>
#include <QFocusEvent>

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
    _firstText = toPlainText();
    _prevText = _firstText;
    QGraphicsTextItem::focusInEvent(event);
}

void WeightTextItem::focusOutEvent(QFocusEvent *event)
{
    if (toPlainText().isEmpty())
        setPlainText(_firstText);
    QTextCursor cursor = textCursor();
    cursor.clearSelection();
    setTextCursor(cursor);
    QGraphicsTextItem::focusOutEvent(event);
    setTextInteractionFlags(Qt::NoTextInteraction);
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

void WeightTextItem::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Escape:
    case Qt::Key_Return:
        break;
    default:
        QGraphicsTextItem::keyReleaseEvent(event);
    }
}

void WeightTextItem::init()
{
    connect(document(), SIGNAL(contentsChanged()), this, SLOT(validateContentsChanging()));
}
