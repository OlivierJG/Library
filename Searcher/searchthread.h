#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

#include <QFileIconProvider>
#include <QThread>
#include <QMutex>
#include <xapian.h>

#include "searchitem.h"

class SearchThread : public QThread
{
/* Data needs to be locked as follows:
 * rw lock: Read and written directly from outside thread, lock at every use
 * w lock: Read directly from outside thread, lock whenever changed, and at points of outside access
 * No need to lock: Boolean variables cannot be invalid, and an incorrect value is meaningless
 */
Q_OBJECT
public:
    SearchThread(QString dbPath, QString filesPath, QObject* parent = 0);
    ~SearchThread();
    SearchItem getSearchResult(int resultIndex) const;
    void setSearchText(QString searchText);
    void setFilterText(QString filterText);
    void finish();
signals:
    void resultCountChanged(int newCount);
private:
    void run();
    void clearAllItems();
    void filterItems();
    void searchForItems();
    SearchItem searchItemFromFile(QString filePath, int score);

    Xapian::Database m_xapianDb;
    Xapian::Enquire m_xapianEnquire;
    Xapian::QueryParser m_xapianQueryParser;
    Xapian::MSet m_xapianMatchSet;
    QFileIconProvider m_iconProvider;
    mutable QMutex m_foundItemsMutex;
    mutable QMutex m_searchTextMutex;
    mutable QMutex m_filterTextMutex;
    QString m_searchText; //Modified outside of thread, rw lock
    QString m_filterText; //Modified outside of thread, rw lock
    QString m_filesPath;
    QList<SearchItem> m_foundItems; //Read outside of thread, w lock
    QList<SearchItem> m_filteredItems;
    //Modified outside of thread, but no need to lock
    volatile bool m_finished;
    volatile bool m_searchTextChanged;
    volatile bool m_filterTextChanged;
};

#endif // SEARCHTHREAD_H
