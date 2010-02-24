#ifndef Searcher_H
#define Searcher_H

#include <QMainWindow>
#include <QLabel>
#include <QTreeView>
#include <QFileSystemModel>
#include <xapian.h>

#include "queryresultview.h"
#include "searchmodel.h"
#include "mylineedit.h"
#include "searchtreeview.h"

#include <ktreewidgetsearchline.h>

using namespace std;

class Searcher : public QMainWindow
{
Q_OBJECT
public:
    Searcher();
    virtual ~Searcher();
private:
    void updateCurrentViewMode();

    MyLineEdit* m_searchText;
    MyLineEdit* m_filterText;
    QLabel* m_contextInfo;
    QTreeView* m_fileSystemTree;
    SearchTreeView* m_searchResultsTreeView;

    QFileSystemModel* m_fileSystemModel;
    SearchModel* m_searchModel;

    QString m_libPath;
private slots:
    void updateContextInfo();
    void searchTextChanged(QString text);
    void filterTextChanged(QString text);
};

#endif // Searcher_H
