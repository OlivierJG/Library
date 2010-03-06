/*Copyright (C) 2010, Olivier JG, olivier.jg@gmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include <QApplication>

#include "enhancedlineedit.h"
#include <QPen>
#include <QPaintEvent>
#include <qstyle.h>

EnhancedLineEdit::EnhancedLineEdit(QString placeHolderText, QWidget* parent): QLineEdit(parent) {
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

void EnhancedLineEdit::setText(QString text)
{
    if (!hasFocus() && text.isEmpty())
        showPlaceholder(true);

    QLineEdit::setText(text);
}

QString EnhancedLineEdit::text()
{
    if (m_showingPlaceholder)
        return "";
    else
        return QLineEdit::text();
}

void EnhancedLineEdit::showPlaceholder(bool show)
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

void EnhancedLineEdit::focusInEvent(QFocusEvent* event)
{
    showPlaceholder(false);
    QLineEdit::focusInEvent(event);
}

void EnhancedLineEdit::focusOutEvent(QFocusEvent* event)
{
    if (text().isEmpty())
        showPlaceholder(true);

    QLineEdit::focusOutEvent(event);
}

