#ifndef INDEXER_H
#define INDEXER_H

#include <QtCore/QObject>
#include <QtCore/QDir>
#include <QtCore/QMap>
#include <xapian.h>

#include "textfilterinterface.h"

class Indexer : public QObject
{
Q_OBJECT
public:
    Indexer(const char * indexPath);
    bool hasStopError() {return m_stopError;};
private:
    void loadTextFilters();
    void buildFileList(QDir inDir);
    void indexFiles();
    QString getMimeType(QString file);
    QString getPathFromIndexDir(QString fullPath);

    Xapian::WritableDatabase m_xapianDb;
    Xapian::TermGenerator m_indexer;
    Xapian::Stem m_stemmer;
    QMap<QString, TextFilterInterface*> m_textFilters;
    bool m_stopError;
    QString m_indexPath;
    QStringList m_filesToIndex;
};

#endif // INDEXER_H
