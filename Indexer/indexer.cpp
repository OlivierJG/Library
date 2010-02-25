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

#include <iostream>
#include <kmimetype.h>
#include <QtCore/QFile>
#include <QtCore/QPluginLoader>
#include <QtCore/QCoreApplication>

#include "indexer.h"

using namespace std;

const char * XAPIAN_DB_DIRECTORY = ".indexdb";
const char * TEXT_FILTERS_DIRECTORY = "TextFilters";

Indexer::Indexer(const char* indexPath)
{
    try
    {
        //First get the root directory of all the files to be indexed
        QDir indexDirectory(indexPath);
        if (!indexDirectory.exists())
        {
            if (!indexDirectory.mkpath(indexDirectory.absolutePath()));
                throw "Unable to create given directory";
        }

        m_indexPath = indexDirectory.absolutePath();

        //Then open/make a subdirectory where the index database is stored
        indexDirectory.mkdir(XAPIAN_DB_DIRECTORY);
        QDir dbDirectory(indexDirectory);
        if (!dbDirectory.cd(XAPIAN_DB_DIRECTORY))
            throw "Unable to create subdirectory for index database";

        loadTextFilters();

        m_xapianDb = Xapian::WritableDatabase(dbDirectory.path().toStdString(), Xapian::DB_CREATE_OR_OPEN);

        buildFileList(indexDirectory);

        indexFiles();
    }
    catch (const Xapian::Error & error)
    {
        cout << "Xapian Exception: " << error.get_msg() << endl;
    }
    catch (const char * error)
    {
        cout << "Is Problem: " << error << endl;
    }
}

void Indexer::loadTextFilters()
{
    //Open the directory containing all the text filter plugins
    QDir textFiltersDir(qApp->applicationDirPath());
    if (!textFiltersDir.cd(TEXT_FILTERS_DIRECTORY))
        throw "Unable to enter text filter plugins directory";

    //Go through all the files contained therein
    foreach (QString fileName, textFiltersDir.entryList(QDir::Files))
    {
        QPluginLoader pluginLoader(textFiltersDir.absoluteFilePath(fileName));
        TextFilterInterface* textFilter = qobject_cast<TextFilterInterface*>(pluginLoader.instance());
        if (textFilter)
        {
            //Found a valid plugin, key all it's supported mimetypes to it
            foreach(QString mimetype, textFilter->handledMimeTypes())
            {
                m_textFilters[mimetype] = textFilter;
            }
        }
    }
}

QString Indexer::getMimeType(QString filePath)
{
    return KMimeType::findByUrl(KUrl(filePath))->name();
}

QString Indexer::getPathFromIndexDir(QString fullPath)
{
    return fullPath.mid(m_indexPath.length());
}

void Indexer::buildFileList(QDir inDir)
{
    //First add all the files in this directory
    QStringList fileList = inDir.entryList(QDir::Files | QDir::Readable);
    for (int i = 0; i < fileList.size(); ++i)
    {
        //Find if this document exists in the DB already
        string pathFromIndexDir = getPathFromIndexDir(inDir.absoluteFilePath(fileList[i])).toStdString();
        Xapian::PostingIterator pi = m_xapianDb.postlist_begin("Q" + pathFromIndexDir);
        cout << pathFromIndexDir << endl;

        //If not, add it
        if (pi == m_xapianDb.postlist_end("Q" + pathFromIndexDir))
            m_filesToIndex << QString(inDir.absoluteFilePath(fileList[i]));
    }

    //Then recurse through all the directories in this one
    QStringList dirList = inDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (int i = 0; i < dirList.size(); ++i)
    {
        if (dirList[i] == XAPIAN_DB_DIRECTORY)
            continue;

        buildFileList(inDir.absolutePath() + QDir::separator() + dirList[i]);
    }
}

void Indexer::indexFiles()
{
    cout << "Found " << m_filesToIndex.size() << " files..." << endl;

    for(int i = 0; i < m_filesToIndex.size(); ++i)
    {
        if (i % 100 == 0)
            m_xapianDb.flush(); //Go easy on the ram

        TextFilterInterface* filter = m_textFilters[getMimeType(m_filesToIndex[i])];

        if (filter)
        {
            //We have a valid text filter for this mimetype, index it
            Xapian::Document newDocument;
            string pathFromIndexDir = getPathFromIndexDir(m_filesToIndex[i]).toStdString();
            newDocument.set_data(pathFromIndexDir);
            newDocument.add_term("Q" + pathFromIndexDir);
            m_indexer.set_document(newDocument);
            m_indexer.index_text(filter->textForFile(m_filesToIndex[i]));
            m_xapianDb.add_document(newDocument);

            cout << i << ": Indexed " << getPathFromIndexDir(m_filesToIndex[i]).toLatin1().data() << endl;
        }
        else
        {
            cout << i << ": No filter found for " << getPathFromIndexDir(m_filesToIndex[i]).toLatin1().data() << endl;
        }
    }
}

#include "indexer.moc"