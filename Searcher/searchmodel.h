#ifndef SEARCHMODEL_H
#define SEARCHMODEL_H

#include <QAbstractItemModel>

#include "searchitem.h"
#include "searchthread.h"

class SearchModel : public QAbstractItemModel
{
Q_OBJECT
public:
    SearchModel(QString dbPath, QString filesPath, QObject* parent = 0);
    ~SearchModel();

    void updateSearchText(QString newText);
    void updateFilterText(QString newText);

    //Implementation of QAbstractItemModel
    QModelIndex index   (int row, int column, const QModelIndex & parent = QModelIndex()) const;
    QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex parent  (const QModelIndex & index) const;
    int rowCount        (const QModelIndex & parent = QModelIndex()) const;
    int columnCount     (const QModelIndex & parent = QModelIndex()) const;
    QVariant data       (const QModelIndex & index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags (const QModelIndex & index) const;
    bool hasChildren    (const QModelIndex & parent = QModelIndex()) const;
private:
    SearchThread* m_searchThread;
    int m_resultCount;
    int m_filterColumn;
private slots:
    void resultCountChanged(int newCount);
};

#endif // SEARCHMODEL_H
