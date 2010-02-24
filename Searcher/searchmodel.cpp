#include "searchmodel.h"
#include <QFileInfo>
#include <QBrush>

const char* MODEL_HEADERS[] = {"Score", "Name", "Size", "Type", "Modified"};

SearchModel::SearchModel(QString dbPath, QString filesPath, QObject* parent):
QAbstractItemModel(parent), m_resultCount(0), m_filterColumn(1)
{
    m_searchThread = new SearchThread(dbPath, filesPath, this);

    connect(m_searchThread, SIGNAL(resultCountChanged(int)), SLOT(resultCountChanged(int)));

    m_searchThread->start();
}

SearchModel::~SearchModel()
{
    m_searchThread->finish();
    m_searchThread->wait();
    delete m_searchThread;
}

int SearchModel::columnCount(const QModelIndex& parent) const
{
    return sizeof(MODEL_HEADERS)/sizeof(char *);
}

QVariant SearchModel::data(const QModelIndex& index, int role) const
{
    SearchItem searchItem(m_searchThread->getSearchResult(index.row()));

    if (searchItem.isValid())
    {
        if (role == Qt::DisplayRole)
        {
            switch (index.column())
            {
                case 0: //Score
                    return QVariant( searchItem.score );
                case 1: //Name
                    return QVariant( searchItem.url );
                case 2: //Size
                    return QVariant( searchItem.size );
                case 3: //Type
                    return QVariant( searchItem.type );
                case 4: //Date Modified
                    return QVariant( searchItem.modified );
            }
        }
        else if (role == Qt::DecorationRole)
        {
            if (index.column() == 1) //Name
                return QVariant( searchItem.icon );
        }
    }

    return QVariant();
}

QModelIndex SearchModel::index(int row, int column, const QModelIndex& parent) const
{
    return createIndex(row, column, 0);
}

QModelIndex SearchModel::parent(const QModelIndex& index) const
{
    return QModelIndex();
}

int SearchModel::rowCount(const QModelIndex& parent) const
{
    return m_resultCount;
}

void SearchModel::updateSearchText(QString newText)
{
    m_searchThread->setSearchText(newText);
}

void SearchModel::updateFilterText(QString newText)
{
    m_searchThread->setFilterText(newText);
}

void SearchModel::resultCountChanged(int newCount)
{
    if (newCount > m_resultCount)
    {
        beginInsertRows(QModelIndex(), m_resultCount, newCount - 1);
        m_resultCount = newCount;
        endInsertRows();
    }
    else if (newCount < m_resultCount)
    {
        beginRemoveRows(QModelIndex(), newCount, m_resultCount - 1);
        m_resultCount = newCount;
        endRemoveRows();
    }
}

Qt::ItemFlags SearchModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant SearchModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
        return QVariant(MODEL_HEADERS[section]);
    else if (role == Qt::DecorationRole && section == m_filterColumn)
        return QIcon::fromTheme("edit-find");
    else
        return QVariant();
}

bool SearchModel::hasChildren(const QModelIndex& parent) const
{
    return (parent == QModelIndex());
}

#include "searchmodel.moc"
