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

#ifndef BASEITEMVIEW_H
#define BASEITEMVIEW_H

#include <QTreeView>
#include <QMouseEvent>

class BaseItemView : public QTreeView
{
Q_OBJECT
public:
    BaseItemView(QWidget* parent = 0);
protected:
    struct BaseItem
    {
        QString url;
        QString type;
        QString displayName;
        QIcon icon;
    };
    virtual BaseItem baseItemFromIndex(QModelIndex index) = 0;
    virtual void mousePressEvent(QMouseEvent* event);
private:
    void openIndex(QModelIndex index, bool inBackground = false);
signals:
    void openItem(QString url, QString type, QString displayName, QIcon icon, bool inBackground = false);
private slots:
    void itemActivated(QModelIndex item);
};

#endif // BASEITEMVIEW_H
