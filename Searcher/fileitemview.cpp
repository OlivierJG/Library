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
#include "fileitemview.h"
#include <QFileIconProvider>

FileItemView::FileItemView(QString filesPath, QFileSystemModel* model, QWidget* parent): BaseItemView(parent)
{
    m_filesPath = filesPath;
    setModel(model);
}

BaseItemView::BaseItem FileItemView::baseItemFromIndex(QModelIndex index)
{
    QFileSystemModel* fileSytemModel = dynamic_cast<QFileSystemModel*>(model());
    QFileInfo fileInfo = fileSytemModel->fileInfo(index);
    if (!fileInfo.isFile())
        return BaseItem();
    
    BaseItem item;
    item.url = fileInfo.absoluteFilePath();
    item.type = fileSytemModel->iconProvider()->type(fileInfo).remove(" File");
    item.displayName = fileInfo.fileName();
    item.icon = fileSytemModel->iconProvider()->icon(fileInfo);
    return item;
}

void FileItemView::setModel(QAbstractItemModel* model)
{
    QTreeView::setModel(model);
    QFileSystemModel* fileSytemModel = dynamic_cast<QFileSystemModel*>(model);
    setColumnWidth(0, 300);
    setRootIndex(fileSytemModel->index(m_filesPath));
}

#include "fileitemview.moc"
