#include <QtCore/QCoreApplication>
#include <iostream>
#include "filterhtml.h"

string FilterHtml::textForFile(QString filePath)
{
    m_textFromHtml = QString("");

    connect(&m_html2Text, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(html2TextFinished(int,QProcess::ExitStatus)));
    connect(&m_html2Text, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));

    m_html2Text.setReadChannel(QProcess::StandardOutput);
    m_html2Text.start("html2text", QStringList() << filePath, QIODevice::ReadOnly);

    if (!m_html2Text.waitForStarted(1000))
        m_html2Text.kill();
    else if (!m_html2Text.waitForFinished())
        m_html2Text.kill();
    else
        return m_textFromHtml.toStdString();
}

QStringList FilterHtml::handledMimeTypes()
{
    return QStringList() << "text/html";
}

void FilterHtml::html2TextFinished(int exitCode, QProcess::ExitStatus exitStatus )
{
    if (!exitStatus == QProcess::NormalExit || exitCode != 0)
        m_textFromHtml = "";
}

void FilterHtml::readyReadStandardOutput()
{
    while( m_html2Text.canReadLine() )
        m_textFromHtml += m_html2Text.readLine() + "\n";
}

Q_EXPORT_PLUGIN2(filterhtml, FilterHtml);

#include "filterhtml.moc"
