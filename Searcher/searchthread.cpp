#include "searchthread.h"

//When searching, update model every this many items found
const int RESULT_BURST_COUNT = 50;
//When searching, display at most this many results
const int MAX_RESULT_COUNT = 500;

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

        msleep(10); //No need to hammer the processor
    }
}

void SearchThread::clearAllItems()
{
    QMutexLocker locker(&m_foundItemsMutex);
    m_foundItems.clear();
    m_filteredItems.clear();
    //Looks like flicker, no need
    //emit resultCountChanged(m_foundItems.size());
}

void SearchThread::filterItems()
{
    int curItems = m_foundItems.size();
    QString filterText;
    {
        QMutexLocker locker(&m_filterTextMutex);
        filterText = m_filterText;
    }
    //First remove non-matching items
    for (int i = m_foundItems.size() - 1; i >= 0 ; --i)
    {
        if (!m_foundItems[i].passesFilter(filterText))
        {
            QMutexLocker locker(&m_foundItemsMutex);
            m_filteredItems.append(m_foundItems.takeAt(i));
        }
    }
    //Then check that filtered items should remain filtered
    for (int i = m_filteredItems.size() - 1; i >= 0; --i)
    {
        if (m_filteredItems[i].passesFilter(filterText))
        {
            QMutexLocker locker(&m_foundItemsMutex);
            m_foundItems.append(m_filteredItems.takeAt(i));
        }
    }

    QMutexLocker locker(&m_foundItemsMutex);
    if(m_foundItems.size() > 1)
    {
        qSort(m_foundItems.begin(), m_foundItems.end());
    }
    //Notify if changed
    if (curItems != m_foundItems.size())
        emit resultCountChanged(m_foundItems.size());
}

void SearchThread::searchForItems()
{
    Xapian::Query xapianQuery;
    {
        QMutexLocker searchTextLocker(&m_searchTextMutex);
        xapianQuery = m_xapianQueryParser.parse_query(m_searchText.toStdString());
    }

    m_xapianEnquire.set_query(xapianQuery);
    m_xapianMatchSet = m_xapianEnquire.get_mset(0, m_xapianDb.get_doccount());

    Xapian::MSetIterator i;
    for (i = m_xapianMatchSet.begin(); i != m_xapianMatchSet.end(); ++i) {
        if (m_searchTextChanged || m_finished || (m_foundItems.size() + m_filteredItems.size()) == MAX_RESULT_COUNT)
            break;

        Xapian::Document doc = i.get_document();
        QString url = m_filesPath + QString(doc.get_data().c_str()).remove(QRegExp("\n.*$")).remove("url=/");

        {
            QMutexLocker itemsLocker(&m_foundItemsMutex);
            QMutexLocker filterTextLocker(&m_filterTextMutex);
            SearchItem newItem = searchItemFromFile(url, (int)i.get_percent());
            if (newItem.passesFilter(m_filterText))
                m_foundItems << newItem;
            else
                m_filteredItems << newItem;
        }

        if (m_foundItems.size() % RESULT_BURST_COUNT == 0)
        {
            emit resultCountChanged(m_foundItems.size());
             //Allow time for thread owner to respond to changes
            if (m_foundItems.size() == RESULT_BURST_COUNT)
                msleep(150); //wait longer on the first to reduce flicker
            else
                msleep(10);
        }
    }

    emit resultCountChanged(m_foundItems.size());
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
