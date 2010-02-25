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

#ifndef Searcher_H
#define Searcher_H

#include <QMainWindow>
#include <QLabel>
#include <QTreeView>
#include <QFileSystemModel>
#include <xapian.h>

#include "queryresultview.h"
#include "searchmodel.h"
#include "mylineedit.h"
#include "searchtreeview.h"

#include <ktreewidgetsearchline.h>

using namespace std;

class Searcher : public QMainWindow
{
Q_OBJECT
public:
    Searcher();
    virtual ~Searcher();
private:
    void updateCurrentViewMode();

    MyLineEdit* m_searchText;
    MyLineEdit* m_filterText;
    QLabel* m_contextInfo;
    QTreeView* m_fileSystemTree;
    SearchTreeView* m_searchResultsTreeView;

    QFileSystemModel* m_fileSystemModel;
    SearchModel* m_searchModel;

    QString m_libPath;
private slots:
    void updateContextInfo();
    void searchTextChanged(QString text);
    void filterTextChanged(QString text);
};

#endif // Searcher_H
