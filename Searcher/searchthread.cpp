#include "searchthread.h"
#include <QLocale>
#include <QDate>

//When searching, update model every this many items found
const int RESULT_BURST_COUNT = 50;

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

        if (m_searchTextChanged)
        {
            clearAllItems();
            m_searchTextChanged = false;
            searchForItems();
        }

        if (m_filterTextChanged)
        {
            m_filterTextChanged = false;
            filterItems();
        }
    }
}

void SearchThread::clearAllItems()
{
    QMutexLocker locker(&m_foundItemsMutex);
    while (!m_foundItems.isEmpty())
        delete m_foundItems.takeLast();
    while (!m_filteredItems.isEmpty())
        delete m_filteredItems.takeLast();
    emit resultCountChanged(m_foundItems.size());
}

void SearchThread::filterItems()
{
    int curItems = m_foundItems.size();
    //First remove non-matching items
    for (int i = m_foundItems.size() - 1; i >= 0 ; i--)
    {
        if (!m_foundItems[i]->url.contains(m_filterText))
        {
            QMutexLocker locker(&m_foundItemsMutex);
            m_filteredItems.append(m_foundItems.takeAt(i));
        }
    }
    //Then check that filtered items should remain filtered
    for (int i = m_filteredItems.size() - 1; i >= 0; i--)
    {
        if (m_filteredItems[i]->url.contains(m_filterText))
        {
            QMutexLocker locker(&m_foundItemsMutex);
            m_foundItems.append(m_filteredItems.takeAt(i));
        }
    }
    //Notify if changed
    if (curItems != m_foundItems.size())
    {
        QMutexLocker locker(&m_foundItemsMutex);
        emit resultCountChanged(m_foundItems.size());
    }
}

void SearchThread::searchForItems()
{
    Xapian::Query xapianQuery;
    {
        QMutexLocker locker(&m_searchTextMutex);
        xapianQuery = m_xapianQueryParser.parse_query(m_searchText.toStdString());
    }

    m_xapianEnquire.set_query(xapianQuery);
    m_xapianMatchSet = m_xapianEnquire.get_mset(0, m_xapianDb.get_doccount());

    Xapian::MSetIterator i;
    for (i = m_xapianMatchSet.begin(); i != m_xapianMatchSet.end(); ++i) {
        if (m_searchTextChanged || m_finished)
            break;

        Xapian::Document doc = i.get_document();
        QString url = m_filesPath + QString(doc.get_data().c_str()).remove(QRegExp("\n.*$")).remove("url=/");

        {
            QMutexLocker itemsLocker(&m_foundItemsMutex);
            SearchItem *newItem = searchItemFromFile(url, (int)i.get_percent());
            m_foundItems << newItem;
        }

        if (m_foundItems.size() % RESULT_BURST_COUNT == 0)
        {
            emit resultCountChanged(m_foundItems.size());
            msleep(10); //Allow time for thread owner to respond to changes
        }
    }

    emit resultCountChanged(m_foundItems.size());
}

void SearchThread::finish()
{
    m_finished = true;
}

SearchItem* SearchThread::searchItemFromFile(QString filePath, int score)
{
    SearchItem* newItem = new SearchItem;
    QFileInfo fileInfo(filePath);

    newItem->score = score;

    newItem->url = fileInfo.fileName();
    newItem->icon = m_iconProvider.icon(fileInfo);

    newItem->size = humanReadableForBytes(fileInfo.size());
    newItem->type = m_iconProvider.type(fileInfo);
    newItem->modified = fileInfo.lastModified().toString();

    return newItem;
}

SearchItem SearchThread::getSearchResult(int resultIndex) const
{
    if (m_foundItems.size() > resultIndex)
        return SearchItem(m_foundItems[resultIndex]);
    else
        return SearchItem();
}

void SearchThread::setSearchText(QString searchText)
{
    QMutexLocker locker(&m_searchTextMutex);
    if (searchText != m_searchText)
    {
        m_searchText = searchText;
        m_searchTextChanged = true;
    }
}

void SearchThread::setFilterText(QString filterText)
{
    QMutexLocker locker(&m_filterTextMutex);
    if (filterText != m_filterText)
    {
        m_filterText = filterText;
        m_filterTextChanged = true;
    }
}

#include "searchthread.moc"
