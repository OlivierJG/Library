#ifndef TEXTFILTERINTERFACE_H
#define TEXTFILTERINTERFACE_H

#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <string>

using namespace std;

class TextFilterInterface
{
    public:
        virtual ~TextFilterInterface() {}
        virtual QStringList handledMimeTypes() = 0;
        virtual string textForFile(QString filePath) = 0;
};

Q_DECLARE_INTERFACE(TextFilterInterface, "app.indexer.Plugin.TextFilterInterface/1.0");

#endif // TEXTFILTERINTERFACE_H
