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

#include "baseitemview.h"

BaseItemView::BaseItemView(QWidget* parent): QTreeView(parent)
{
    connect(this, SIGNAL(activated(QModelIndex)), this, SLOT(itemActivated(QModelIndex)));
}

void BaseItemView::itemActivated(QModelIndex index)
{
    openIndex(index);
}

void BaseItemView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MidButton)
        openIndex(indexAt(event->pos()), true);

    QTreeView::mousePressEvent(event);
}

void BaseItemView::openIndex(QModelIndex index, bool inBackground)
{
    BaseItem item = baseItemFromIndex(index);
    if (!item.url.isEmpty())
        emit openItem(item.url, item.type, item.displayName, item.icon, inBackground);
}

#include "baseitemview.moc"
