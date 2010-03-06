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
#include "htmlviewcreator.h"
#include <QVBoxLayout>
#include <QDebug>

HtmlView::HtmlView(QUrl url, QWidget* parent): QWidget(parent)
{
    m_textBrowser = new QTextBrowser(this);
    m_textBrowser->setSource(QUrl(url));
    m_findText = new QLineEdit(this);
    m_findNext = new QPushButton("Find Next", this);
    m_findPrevious = new QPushButton("Find Previous", this);
    m_searchResultHighlighter = new HtmlViewSearchResultHighlighter(m_textBrowser);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(m_textBrowser);
    QHBoxLayout* hlayout = new QHBoxLayout();
    hlayout->addWidget(m_findText);
    hlayout->addWidget(m_findNext);
    hlayout->addWidget(m_findPrevious);
    layout->addLayout(hlayout);
    setLayout(layout);

    connect(m_findText, SIGNAL(textChanged(QString)), this, SLOT(findTextChanged(QString)));
    connect(m_findText, SIGNAL(returnPressed()), m_findNext, SIGNAL(pressed()));
    connect(m_findNext, SIGNAL(pressed()), this, SLOT(findNextPressed()));
    connect(m_findPrevious, SIGNAL(pressed()), this, SLOT(findPreviousPressed()));
}

void HtmlView::findNextPressed()
{
    findText(m_findText->text(), Next);
}

void HtmlView::findPreviousPressed()
{
    findText(m_findText->text(), Previous);
}

void HtmlView::findTextChanged(QString text)
{
    m_searchResultHighlighter->setHighlightedText(text);
    findText(text, Extend);
}

bool HtmlView::findText(QString text, FindOptions options)
{
    QTextCursor textCursor;

    //First just search as requested
    switch (options)
    {
    case Extend:
        textCursor = m_textBrowser->document()->find(text, m_textBrowser->textCursor().selectionStart());
        break;
    case Next:
        textCursor = m_textBrowser->document()->find(text, m_textBrowser->textCursor().selectionEnd());
        break;
    case Previous:
        textCursor = m_textBrowser->document()->find(text, m_textBrowser->textCursor().selectionStart(), QTextDocument::FindBackward);
        break;
    }

    //If not found, search from document start or end
    if (textCursor.isNull())
    {
        switch (options)
        {
        case Extend:
        case Next:
            textCursor = m_textBrowser->document()->find(text, 0);
            break;
        case Previous:
            textCursor = m_textBrowser->document()->find(text, m_textBrowser->document()->characterCount(), QTextDocument::FindBackward);
            break;
        }
    }

    m_textBrowser->setTextCursor(textCursor);

    if (!textCursor.isNull())
        return true;

    return false;
}

void HtmlViewSearchResultHighlighter::highlightBlock(const QString& text)
{
    if (m_textToHighlight.isEmpty())
        return;

    QTextCharFormat highlightFormat;
    highlightFormat.setBackground(Qt::yellow);

    int index = text.indexOf(m_textToHighlight, 0, Qt::CaseInsensitive);

    while (index >= 0) {
        setFormat(index, m_textToHighlight.length(), highlightFormat);
        index = text.indexOf(m_textToHighlight, index + m_textToHighlight.length(), Qt::CaseInsensitive);
    }
}

void HtmlViewSearchResultHighlighter::setHighlightedText(QString text)
{
    m_textToHighlight = text;
    rehighlight();
}

QStringList HtmlViewCreator::handledMimeTypes()
{
    return QStringList() << "htm" << "html";
}

QWidget* HtmlViewCreator::createViewForFile(QUrl url, QWidget* parent)
{
    HtmlView* view = new HtmlView(url, parent);
    return view;
}

Q_EXPORT_PLUGIN2(htmlviewcreator, HtmlViewCreator);

#include "htmlviewcreator.moc"
