#ifndef FILTERPLAINTEXT_H
#define FILTERPLAINTEXT_H

#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtCore/QtPlugin>
#include <../textfilterinterface.h>

using namespace std;

class FilterPlainText : public QObject, public TextFilterInterface
{
    Q_OBJECT
    Q_INTERFACES(TextFilterInterface)
public:
    string textForFile(QString filePath);
    QStringList handledMimeTypes();
};

#endif // FILTERPLAINTEXT_H
