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

#ifndef SEARCHITEM_H
#define SEARCHITEM_H

#include <QString>
#include <QIcon>
#include <QVariant>
#include <QLocale>
#include <QDate>
#include <QFileInfo>

struct SearchItem
{
    SearchItem() : url(""), score(0), size(0), modified(), type("") { }
    SearchItem(SearchItem* other)
    : url(other->url), score(other->score),
      size(other->size), modified(other->modified),
      type(other->type), icon(other->icon) { }
    QString url;
    int score;
    qint64 size;
    QDateTime modified;
    QString type;
    QIcon icon;

    bool isValid()
    {
        return (!url.isEmpty());
    }

    QVariant dataForColumn(int column, int role)
    {
        if (!isValid())
            return QVariant();

        if (role == Qt::DisplayRole)
        {
            switch (column)
            {
                case 0:
                    return QVariant( QString::number(score) + "%" );
                case 1:
                    return QVariant( QFileInfo(url).fileName() );
                case 2:
                    return QVariant( humanReadableForBytes(size) );
                case 3:
                    return QVariant( type );
                case 4:
                    return QVariant( modified );
            }
        }
        else if (role == Qt::DecorationRole)
        {
            if (column == 1)
                return QVariant( icon );
        }

        return QVariant();
    }

    bool passesFilter(QString filter)
    {
        QStringList filterParts = filter.split(" ", QString::SkipEmptyParts);
        QString mainFilter;
        foreach (QString filterPart, filterParts)
        {
            if (filterPart.startsWith("larger:")) {
                if (size <= filterPart.remove(0, 7).toInt())
                    return false;
            }
            else if (filterPart.startsWith("smaller:")) {
                if (size >= filterPart.remove(0, 8).toInt())
                    return false;
            }
            else if (filterPart.startsWith("type:")) {
                if (type.remove(" File") != filterPart.remove(0, 5))
                    return false;
            }
            else if (filterPart.startsWith("typenot:")) {
                if (type == filterPart.remove(0, 8))
                    return false;
            }
            else if (filterPart.startsWith("changedafter:")) {
                //TODO: fill this in
            }
            else if (filterPart.startsWith("changedbefore:")) {
                //TODO: fill this in
            }
            else if (filterPart.startsWith("scoreatleast:")) {
                if (score < filterPart.remove(0, 13).toInt())
                    return false;
            }
            else {
                mainFilter.append(filterPart.toLower() + " ");
            }

            if (!url.toLower().contains(mainFilter.trimmed()))
                return false;
        }
        return true;
    }

    bool operator<(const SearchItem& other) const
    {
        return score < other.score;
    }

private:
    //Function contents shamelessly ripped from Qt's qdirmodel.cpp 4.5.3 implementation
    //in order to match with the QFileSystemModel
    QString humanReadableForBytes(quint64 bytes)
    {
        // According to the Si standard KB is 1000 bytes, KiB is 1024
        // but on windows sizes are calulated by dividing by 1024 so we do what they do.
        const quint64 kb = 1024;
        const quint64 mb = 1024 * kb;
        const quint64 gb = 1024 * mb;
        const quint64 tb = 1024 * gb;

        if (bytes >= tb)
            return QLocale().toString(bytes / tb) + QString::fromLatin1(" TB");
        if (bytes >= gb)
            return QLocale().toString(bytes / gb) + QString::fromLatin1(" GB");
        if (bytes >= mb)
            return QLocale().toString(bytes / mb) + QString::fromLatin1(" MB");
        if (bytes >= kb)
            return QLocale().toString(bytes / kb) + QString::fromLatin1(" KB");
        return QLocale().toString(bytes) + QString::fromLatin1(" bytes");
    }

};

#endif