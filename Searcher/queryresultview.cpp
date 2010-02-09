#include "queryresultview.h"
#include <QScrollBar>

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
    QString url;
    QString title;
    QString type;
    int size;

    QStringList queryProperties = dbDescription.split("\n");
    foreach(QString queryProperty, queryProperties)
    {
        if (queryProperty.contains("url="))
            url = queryProperty.remove("url=/"); //Remove the leading "/" TODO: remove it in the indexer
        if (queryProperty.contains("caption="))
            title = queryProperty.remove("caption=");
        if (queryProperty.contains("type="))
            type = queryProperty.remove("type=");
        if (queryProperty.contains("size="))
            size = queryProperty.remove("size=").toInt();
    }

    QString htmlText;
    htmlText += "<b>" + title + "</b><br/>\n";
    htmlText += "<a href=\"" + url + "\">" + url + "</a><br/>\n";
    htmlText += "Type: " + type + ", Size: " + QString::number(size) + "<br/>\n";
    htmlText += "Match Rating: <b>" + QString::number(score) + "%</b><br/>\n";

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
