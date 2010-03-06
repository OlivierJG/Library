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
#ifndef HTMLVIEWCREATOR_H
#define HTMLVIEWCREATOR_H
#include <QObject>
#include <qplugin.h>
#include <QTextBrowser>
#include <QLineEdit>
#include <QPushButton>
#include <QSyntaxHighlighter>

#include <../embeddedviewcreatorinterface.h>

class HtmlViewCreator : public QObject, public EmbeddedViewCreatorInterface
{
Q_OBJECT
Q_INTERFACES(EmbeddedViewCreatorInterface)
public:
    virtual QStringList handledMimeTypes();
    virtual QWidget* createViewForFile(QUrl url, QWidget* parent);
};

class HtmlViewSearchResultHighlighter : public QSyntaxHighlighter
{
public:
    HtmlViewSearchResultHighlighter(QTextEdit* parent) : QSyntaxHighlighter(parent) {};
    void setHighlightedText(QString text);
protected:
    virtual void highlightBlock(const QString& text);
private:
    QString m_textToHighlight;
};

class HtmlView : public QWidget
{
Q_OBJECT
public:
    HtmlView(QUrl url, QWidget* parent = 0);
private:
    enum FindOptions
    {
        Previous,
        Next,
        Extend,
    };
    QTextBrowser *m_textBrowser;
    QLineEdit *m_findText;
    QPushButton *m_findNext;
    QPushButton *m_findPrevious;
    HtmlViewSearchResultHighlighter *m_searchResultHighlighter;

    bool findText(QString text, FindOptions options);
public slots:
    void findTextChanged(QString);
    void findNextPressed();
    void findPreviousPressed();
};

#endif // HTMLVIEWCREATOR_H
