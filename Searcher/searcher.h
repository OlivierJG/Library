#ifndef Searcher_H
#define Searcher_H

#include <QtGui/QMainWindow>
#include <QtGui/QLineEdit>
#include <QtGui/QLabel>
#include <QTreeView>
#include <QtGui/QTextEdit>
#include <xapian.h>
#include <iostream>

#include "queryresultview.h"

using namespace std;

class Searcher : public QMainWindow
{
Q_OBJECT
public:
    Searcher();
    bool hasException() { return m_hasException; }
    virtual ~Searcher();
private:
    QLineEdit* m_searchText;
    QLineEdit* m_searchInfo;
    QLineEdit* m_browserSearch;
    QTreeView* m_fileBrowser;
    QueryResultView* m_queryResults;
    Xapian::Database* m_xapianDb;
    Xapian::Enquire* m_xapianEnquire;
    Xapian::QueryParser* m_xapianQueryParser;
    Xapian::MSet m_xapianMatchSet;
    bool m_hasException;

    void initXapian();
    void updateSearchInfo();

    friend struct SearchError;
    struct SearchError {
        SearchError(QString errorMessage, Searcher* searcher) {
            searcher->m_hasException = true;
            message = errorMessage;
        }
        QString message;
    };
private slots:
    void updateSearchQuery(QString query);
    void getMoreQueryResults();
    void fillResultsWidget();
    void searchInBrowser(QString text, bool findNext = false);
    void searchInBrowserNext();
};

#endif // Searcher_H
