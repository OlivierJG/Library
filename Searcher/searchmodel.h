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

#ifndef SEARCHMODEL_H
#define SEARCHMODEL_H

#include <QAbstractItemModel>

#include "searchitem.h"
#include "searchthread.h"

class SearchModel : public QAbstractItemModel
{
Q_OBJECT
public:
    SearchModel(QString dbPath, QString filesPath, QObject* parent = 0);
    ~SearchModel();

    void updateSearchText(QString newText);
    void updateFilterText(QString newText);

    //Implementation of QAbstractItemModel
    QModelIndex index   (int row, int column, const QModelIndex & parent = QModelIndex()) const;
    QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex parent  (const QModelIndex & index) const;
    int rowCount        (const QModelIndex & parent = QModelIndex()) const;
    int columnCount     (const QModelIndex & parent = QModelIndex()) const;
    QVariant data       (const QModelIndex & index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags (const QModelIndex & index) const;
    bool hasChildren    (const QModelIndex & parent = QModelIndex()) const;
private:
    SearchThread* m_searchThread;
    int m_resultCount;
    int m_filterColumn;
private slots:
    void resultCountChanged(int newCount);
};

#endif // SEARCHMODEL_H
