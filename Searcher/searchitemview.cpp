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

#include "searchitemview.h"

SearchItemView::SearchItemView(SearchModel* model, QWidget* parent): BaseItemView(parent)
{
    setModel(model);
}

void SearchItemView::setModel(QAbstractItemModel *model)
{
    QTreeView::setModel(model);
    setColumnWidth(1, 300); //Default second column to a more useful width
}

BaseItemView::BaseItem SearchItemView::baseItemFromIndex(QModelIndex index)
{
    QModelIndex nameIndex = index.sibling(index.row(), SearchItem::Name);
    QModelIndex typeIndex = index.sibling(index.row(), SearchItem::Type);
    if (!nameIndex.isValid() || !typeIndex.isValid())
        return BaseItem();

    BaseItem item;
    item.url = nameIndex.data(SearchItem::DataRole).toString();
    item.type = typeIndex.data(SearchItem::DataRole).toString();
    item.displayName = nameIndex.data(Qt::DisplayRole).toString();
    item.icon = nameIndex.data(Qt::DecorationRole).value<QIcon>();

    return item;
}

#include "searchitemview.moc"
