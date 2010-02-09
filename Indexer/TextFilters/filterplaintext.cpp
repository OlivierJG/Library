#include "filterplaintext.h"

string FilterPlainText::textForFile(QString filePath)
{
    string whatever = "";
    return whatever;
}

QStringList FilterPlainText::handledMimeTypes()
{
    return QStringList();
}

Q_EXPORT_PLUGIN2(filterplaintext, FilterPlainText);

#include "filterplaintext.moc"
