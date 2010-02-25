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

#ifndef INDEXER_H
#define INDEXER_H

#include <QtCore/QObject>
#include <QtCore/QDir>
#include <QtCore/QMap>
#include <xapian.h>

#include "textfilterinterface.h"

class Indexer : public QObject
{
Q_OBJECT
public:
    Indexer(const char * indexPath);
    bool hasStopError() {return m_stopError;};
private:
    void loadTextFilters();
    void buildFileList(QDir inDir);
    void indexFiles();
    QString getMimeType(QString file);
    QString getPathFromIndexDir(QString fullPath);

    Xapian::WritableDatabase m_xapianDb;
    Xapian::TermGenerator m_indexer;
    Xapian::Stem m_stemmer;
    QMap<QString, TextFilterInterface*> m_textFilters;
    bool m_stopError;
    QString m_indexPath;
    QStringList m_filesToIndex;
};

#endif // INDEXER_H
