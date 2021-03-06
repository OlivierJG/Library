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

#include "searchmodel.h"
#include <QFileInfo>
#include <QBrush>

const char* MODEL_HEADERS[] = {"Score", "Name", "Size", "Type", "Modified"};

SearchModel::SearchModel(QString dbPath, QString filesPath, QObject* parent):
QAbstractItemModel(parent), m_resultCount(0), m_filterColumn(1)
{
    m_searchThread = new SearchThread(dbPath, filesPath, this);

    connect(m_searchThread, SIGNAL(resultCountChanged(int)), SLOT(resultCountChanged(int)));

    m_searchThread->start();
}

SearchModel::~SearchModel()
{
    m_searchThread->finish();
    m_searchThread->wait();
    delete m_searchThread;
}

int SearchModel::columnCount(const QModelIndex& parent) const
{
    return sizeof(MODEL_HEADERS)/sizeof(char *);
}

QVariant SearchModel::data(const QModelIndex& index, int role) const
{
    SearchItem searchItem(m_searchThread->getSearchResult(index.row()));

    return searchItem.dataForColumn(index.column(), role);
}

QModelIndex SearchModel::index(int row, int column, const QModelIndex& parent) const
{
    return createIndex(row, column, 0);
}

QModelIndex SearchModel::parent(const QModelIndex& index) const
{
    return QModelIndex();
}

int SearchModel::rowCount(const QModelIndex& parent) const
{
    return m_resultCount;
}

void SearchModel::updateSearchText(QString newText)
{
    m_searchThread->setSearchText(newText);
}

void SearchModel::updateFilterText(QString newText)
{
    m_searchThread->setFilterText(newText);
}

void SearchModel::resultCountChanged(int newCount)
{
    if (newCount > m_resultCount)
    {
        beginInsertRows(QModelIndex(), m_resultCount, newCount - 1);
        m_resultCount = newCount;
        endInsertRows();
    }
    else if (newCount < m_resultCount)
    {
        beginRemoveRows(QModelIndex(), newCount, m_resultCount - 1);
        m_resultCount = newCount;
        endRemoveRows();
    }
}

Qt::ItemFlags SearchModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant SearchModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
        return QVariant(MODEL_HEADERS[section]);

    return QVariant();
}

bool SearchModel::hasChildren(const QModelIndex& parent) const
{
    return (parent == QModelIndex());
}

#include "searchmodel.moc"
