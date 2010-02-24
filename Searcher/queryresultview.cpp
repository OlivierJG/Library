#include "queryresultview.h"
#include <QtGui/QScrollBar>

QueryResultView::QueryResultView(QString urlBasePath, QWidget* parent): QTextBrowser(parent), m_resultCount(0)
{
    setReadOnly(true);
    setSearchPaths(QStringList() << urlBasePath);
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(vscrollValueChanged(int)));
}

void QueryResultView::addResult(QString dbDescription, int score)
{
    int vScrollPos = verticalScrollBar()->value();
    blockSignals(true);
    append(getHtmlForResult(dbDescription, score));
    blockSignals(false);
    verticalScrollBar()->setValue(vScrollPos);

    m_resultCount++;
}

QString QueryResultView::getHtmlForResult(QString dbDescription, int score)
{


    QString htmlText;
/*    htmlText += "<b>" + title + "</b><br/>\n";
    htmlText += "<a href=\"" + url + "\">" + url + "</a><br/>\n";
    htmlText += "Type: " + type + ", Size: " + QString::number(size) + "<br/>\n";
    htmlText += "Match Rating: <b>" + QString::number(score) + "%</b><br/>\n";*/

    return htmlText;
}

int QueryResultView::resultCount()
{
    return m_resultCount;
}

void QueryResultView::clear()
{
    m_resultCount = 0;
    QTextEdit::clear();
    append("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">");
}

void QueryResultView::vscrollValueChanged(int newValue)
{
    if (newValue == verticalScrollBar()->maximum())
        scrolledToEnd();
}

void QueryResultView::resizeEvent(QResizeEvent* event)
{
    QTextBrowser::resizeEvent(event);
    resized();
}

#include "queryresultview.moc"
