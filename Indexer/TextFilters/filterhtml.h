#ifndef FILTERHTML_H
#define FILTERHTML_H

#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtCore/QtPlugin>
#include <QtCore/QProcess>
#include <../textfilterinterface.h>

using namespace std;

class FilterHtml : public QObject, public TextFilterInterface
{
    Q_OBJECT
    Q_INTERFACES(TextFilterInterface)
public:
    string textForFile(QString filePath);
    QStringList handledMimeTypes();
public slots:
    void readyReadStandardOutput();
    void html2TextFinished(int exitCode, QProcess::ExitStatus exitStatus);
private:
    QProcess m_html2Text;
    QString m_textFromHtml;
};

#endif // FILTERHTML_H
