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
