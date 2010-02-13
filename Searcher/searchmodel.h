#ifndef SEARCHMODEL_H
#define SEARCHMODEL_H

#include <QStandardItemModel>
#include <QFileIconProvider>
#include <xapian.h>

class SearchModel : public QStandardItemModel
{
Q_OBJECT
public:
    SearchModel(QString dbPath, QString filesPath, QObject* parent = 0);
    int estimatedResultCount();
    void getMoreResults();
public slots:
    void updateSearchText(QString newText);
private:
    void setHorizontalHeaderText();

    Xapian::Database m_xapianDb;
    Xapian::Enquire m_xapianEnquire;
    Xapian::QueryParser m_xapianQueryParser;
    Xapian::MSet m_xapianMatchSet;
    QString m_filesPath;
    QFileIconProvider m_iconProvider;
};

#endif // SEARCHMODEL_H
