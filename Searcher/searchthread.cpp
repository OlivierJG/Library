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

#include "searchthread.h"

//When searching, update model every this many items found
const int RESULT_BURST_COUNT = 50;
//When searching, display at most this many results
const int MAX_RESULT_COUNT = 500;
const int SHORT_SLEEP = 10;
const int LONG_SLEEP = 150;

SearchThread::SearchThread(QString dbPath, QString filesPath, QObject* parent):
QThread(parent),
m_xapianDb(dbPath.toStdString()),
m_xapianEnquire(m_xapianDb),
m_iconProvider(),
m_finished(false),
m_searchTextChanged(false),
m_filterTextChanged(false),
m_filesPath(filesPath)
{

}

SearchThread::~SearchThread()
{
    clearAllItems();
}

void SearchThread::run()
{
    forever {
        if (m_finished)
            return;

        if (m_searchTextChanged || m_filterTextChanged)
        {
            clearAllItems();
            searchForItems();
        }

        msleep(10); //No need to hammer the processor
    }
}

void SearchThread::clearAllItems()
{
    {
        QMutexLocker locker(&m_foundItemsMutex);
        m_foundItems.clear();
    }
    //Don't emit resultCountChanged, looks like flicker since immediately after clearing it gets filled again
    //emit resultCountChanged(m_foundItems.size());
    //sleep(10);
}

void SearchThread::searchForItems()
{
    Xapian::Query xapianQuery;
    {
        QMutexLocker searchTextLocker(&m_searchAndFilterTextMutex);
        xapianQuery = m_xapianQueryParser.parse_query(m_searchText.toStdString());
    }

    m_xapianEnquire.set_query(xapianQuery);

    if (m_searchTextChanged)
        m_xapianMatchSet = m_xapianEnquire.get_mset(0, m_xapianDb.get_doccount());

    m_filterTextChanged = false;
    m_searchTextChanged = false;

    emit searchStarted();

    Xapian::MSetIterator i;
    int filteredCount = 0;
    for (i = m_xapianMatchSet.begin(); i != m_xapianMatchSet.end(); ++i)
    {

        //Test that current state is still valid
        if (m_filterTextChanged ||
            m_searchTextChanged ||
            m_finished ||
            m_foundItems.size() == MAX_RESULT_COUNT)
            break;

        Xapian::Document doc = i.get_document();
        QString url = m_filesPath + QString(doc.get_data().c_str()).remove(QRegExp("\n.*$")).remove("url=/");

        {
            QMutexLocker itemsLocker(&m_foundItemsMutex);
            QMutexLocker filterTextLocker(&m_searchAndFilterTextMutex);
            SearchItem newItem = searchItemFromFile(url, (int)i.get_percent());
            if (newItem.passesFilter(m_filterText))
                m_foundItems << newItem;
            else
                ++filteredCount;
        }

        //Sometimes thousands of results must be searched only to filter tens, filteredCount is included
        //to make sure that it doesn't go through all the results before updating the UI
        if (m_foundItems.size() > 0 && m_foundItems.size() % RESULT_BURST_COUNT == 0)
        {
            emit resultCountChanged(m_foundItems.size());

            //Allow time for thread owner to respond to changes
            if (m_foundItems.size() == RESULT_BURST_COUNT)
                msleep(LONG_SLEEP); //wait longer on the first to reduce flicker
            else if (m_foundItems.size() > RESULT_BURST_COUNT)
                msleep(SHORT_SLEEP);
        }
        else if (filteredCount > 0 && filteredCount % RESULT_BURST_COUNT == 0)
        {
            emit resultCountChanged(m_foundItems.size());

            if (filteredCount == RESULT_BURST_COUNT)
                msleep(LONG_SLEEP);
            else if (filteredCount > RESULT_BURST_COUNT)
                msleep(SHORT_SLEEP);
        }
    }

    emit resultCountChanged(m_foundItems.size());

    emit searchEnded();
}

void SearchThread::finish()
{
    m_finished = true;
}

SearchItem SearchThread::searchItemFromFile(QString filePath, int score)
{
    SearchItem newItem;
    QFileInfo fileInfo(filePath);

    newItem.score = score;

    newItem.url = fileInfo.absoluteFilePath();
    newItem.icon = m_iconProvider.icon(fileInfo);

    newItem.size = fileInfo.size();
    newItem.type = m_iconProvider.type(fileInfo);
    newItem.modified = fileInfo.lastModified();

    return newItem;
}

SearchItem SearchThread::getSearchResult(int resultIndex) const
{
    QMutexLocker locker(&m_foundItemsMutex);
    if (m_foundItems.size() > resultIndex)
        return SearchItem(m_foundItems[resultIndex]);
    else
        return SearchItem();
}

void SearchThread::setSearchText(QString searchText)
{
    QMutexLocker locker(&m_searchAndFilterTextMutex);
    if (searchText != m_searchText)
    {
        m_searchText = searchText;
        m_searchTextChanged = true;
    }
}

void SearchThread::setFilterText(QString filterText)
{
    QMutexLocker locker(&m_searchAndFilterTextMutex);
    if (filterText != m_filterText)
    {
        m_filterText = filterText;
        m_filterTextChanged = true;
    }
}

#include "searchthread.moc"
