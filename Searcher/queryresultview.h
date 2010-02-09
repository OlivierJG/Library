#ifndef QUERYRESULTVIEW_H
#define QUERYRESULTVIEW_H

#include <QTextBrowser>
#include <QString>
#include <QUrl>
#include <iostream>

class QueryResultView : public QTextBrowser
{
Q_OBJECT
public:
    QueryResultView(QString urlBasePath, QWidget* parent = 0);
    void addResult(QString dbDescription, int score);
    int resultCount();
    void clear();

signals:
    void scrolledToEnd();
    void resized();
public slots:
    void vscrollValueChanged(int newValue);

private:
    void resizeEvent(QResizeEvent * event);
    QString getHtmlForResult(QString dbDescription, int score);
    int m_resultCount;
};
#endif // QUERYRESULTVIEW_H
