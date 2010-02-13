#include "searchmodel.h"
#include <QFileInfo>
#include <QDate>

const int GET_MORE_RESULTS_COUNT = 5;

SearchModel::SearchModel(QString dbPath, QString filesPath, QObject* parent):
    QStandardItemModel(parent),
    m_xapianDb(dbPath.toStdString()),
    m_xapianEnquire(m_xapianDb),
    m_filesPath(filesPath),
    m_iconProvider()
{
    setHorizontalHeaderText();
}

void SearchModel::setHorizontalHeaderText()
{
    setHorizontalHeaderLabels(QStringList() << "Score" << "Name" << "Size" << "Type" << "Date Modified");
}

int SearchModel::estimatedResultCount()
{
    return m_xapianMatchSet.get_matches_estimated();
}

void SearchModel::getMoreResults()
{
    m_xapianMatchSet = m_xapianEnquire.get_mset(rowCount(), GET_MORE_RESULTS_COUNT);

    Xapian::MSetIterator i;
    for (i = m_xapianMatchSet.begin(); i != m_xapianMatchSet.end(); ++i) {
        Xapian::Document doc = i.get_document();
        QStringList queryProperties = QString(doc.get_data().c_str()).split("\n");
        QString url;
        foreach(QString queryProperty, queryProperties)
        {
            if (queryProperty.contains("url=/"))
                url = queryProperty.remove("url=/");
        }
        QFileInfo fileInfo(m_filesPath + url);
        QList<QStandardItem*> queryItems;

        queryItems << new QStandardItem(QString::number(i.get_percent()));

        QStandardItem* name = new QStandardItem(fileInfo.fileName());
        name->setIcon(m_iconProvider.icon(fileInfo));
        queryItems << name;

        queryItems << new QStandardItem(QString::number(fileInfo.size()));
        queryItems << new QStandardItem(m_iconProvider.type(fileInfo));
        queryItems << new QStandardItem(fileInfo.lastModified().toString());

        appendRow(queryItems);
    }
}

void SearchModel::updateSearchText(QString newText)
{
    Xapian::Query xapianQuery = m_xapianQueryParser.parse_query(newText.toStdString());
    m_xapianEnquire.set_query(xapianQuery);
    clear();
    setHorizontalHeaderText();
}

#include "searchmodel.moc"
