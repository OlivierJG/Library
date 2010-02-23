#include <QApplication>

#include "mylineedit.h"
#include <QPen>
#include <QPaintEvent>
#include <qstyle.h>

MyLineEdit::MyLineEdit(QString placeHolderText, QWidget* parent): QLineEdit(parent) {
    m_placeholderText = placeHolderText;
    m_placeholderFont = font();
    m_placeholderFont.setItalic(true);
    QColor placeholderColor = palette().color(QPalette::Text);
    placeholderColor.setAlpha(128);
    m_placeholderPalette.setColor(QPalette::Text, placeholderColor);

    m_normalFont = font();
    m_normalPalette = palette();

    showPlaceholder(!hasFocus());
}

void MyLineEdit::setText(QString text)
{
    if (!hasFocus() && text.isEmpty())
        showPlaceholder(true);

    QLineEdit::setText(text);
}

QString MyLineEdit::text()
{
    if (m_showingPlaceholder)
        return "";
    else
        return QLineEdit::text();
}

void MyLineEdit::showPlaceholder(bool show)
{
    blockSignals(true);
    if (show)
    {
        setPalette(m_placeholderPalette);
        setFont(m_placeholderFont);
        setText(m_placeholderText);
    }
    else
    {
        setPalette(m_normalPalette);
        setFont(m_normalFont);
        if (m_showingPlaceholder)
            setText("");
    }
    blockSignals(false);

    m_showingPlaceholder = show;
}

void MyLineEdit::focusInEvent(QFocusEvent* event)
{
    showPlaceholder(false);
    QLineEdit::focusInEvent(event);
}

void MyLineEdit::focusOutEvent(QFocusEvent* event)
{
    if (text().isEmpty())
        showPlaceholder(true);

    QLineEdit::focusOutEvent(event);
}

