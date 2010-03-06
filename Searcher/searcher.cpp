/*Copyright (C) 2010, Olivier JG, olivier.jg@gmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include "searcher.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QPluginLoader>
#include <QSplitter>

Searcher::Searcher()
{
    m_libPath = "../../Library/";

    resize(800,600);

    m_searchModel = new SearchModel("../../.libindex/", m_libPath, this);

    m_fileSystemModel = new QFileSystemModel(this);
    m_fileSystemModel->setRootPath(m_libPath);
    m_fileSystemModel->setNameFilterDisables(false);

    setCentralWidget(new QWidget);

    QSplitter* splitter = new QSplitter(Qt::Horizontal, centralWidget());
    QWidget* leftWidget = new QWidget(splitter);
    QWidget* rightWidget = new QWidget(splitter);

    m_contextInfo = new QLabel(leftWidget);
    m_fileItemView = new FileItemView(m_libPath, m_fileSystemModel, leftWidget);
    m_searchItemView = new SearchItemView(m_searchModel, leftWidget);
    m_searchItemView->setVisible(false);
    m_filterText = new EnhancedLineEdit("Filter...", leftWidget);
    m_searchText = new EnhancedLineEdit("Search...", leftWidget);

    m_tabbedFileView = new EmbeddedViewTabber(rightWidget);

    QVBoxLayout* leftlayout = new QVBoxLayout(leftWidget);
    leftlayout->addWidget(m_contextInfo);
    leftlayout->addWidget(m_fileItemView);
    leftlayout->addWidget(m_searchItemView);
    leftlayout->addWidget(m_filterText);
    leftlayout->addWidget(m_searchText);
    leftWidget->setLayout(leftlayout);

    QVBoxLayout* rightlayout = new QVBoxLayout(rightWidget);
    rightlayout->addWidget(m_tabbedFileView);
    rightWidget->setLayout(rightlayout);

    splitter->addWidget(leftWidget);
    splitter->addWidget(rightWidget);
    splitter->setCollapsible(1, false);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(splitter);
    centralWidget()->setLayout(layout);

    connect(m_searchText, SIGNAL(textChanged(QString)), this, SLOT(searchTextChanged(QString)));
    connect(m_filterText, SIGNAL(textChanged(QString)), this, SLOT(filterTextChanged(QString)));
    connect(m_searchModel, SIGNAL(rowsInserted(const QModelIndex,int,int)), this, SLOT(updateContextInfo()));
    connect(m_searchModel, SIGNAL(rowsRemoved(const QModelIndex,int,int)), this, SLOT(updateContextInfo()));
    connect(m_searchItemView, SIGNAL(openItem(QString,QString,QString,QIcon,bool)), this,
            SLOT(openItem(QString,QString,QString,QIcon,bool)));
    connect(m_fileItemView, SIGNAL(openItem(QString,QString,QString,QIcon,bool)), this,
            SLOT(openItem(QString,QString,QString,QIcon,bool)));

    updateContextInfo();
    loadEmbeddedViewCreators();
}

void Searcher::searchTextChanged(QString text)
{
    m_searchModel->updateSearchText(text);
    updateCurrentViewMode();
    updateContextInfo();
}

void Searcher::filterTextChanged(QString text)
{
    m_searchModel->updateFilterText(text);
}

void Searcher::updateContextInfo()
{
    if (m_searchItemView->isVisible())
        m_contextInfo->setText(QString::number(m_searchModel->rowCount()) +
                               " result" +
                               (m_searchModel->rowCount() == 1 ? " from" : "s from") +
                               m_libPath);
    else
        m_contextInfo->setText("Browsing Library in " + m_libPath);
}

void Searcher::loadEmbeddedViewCreators()
{
    //Open the directory containing all the embedded viewer plugins
    QDir embeddedViewCreatorsDir(qApp->applicationDirPath());
    if (!embeddedViewCreatorsDir.cd("EmbeddedViewCreators"))
        return;

    //Go through all the files contained therein
    foreach (QString fileName, embeddedViewCreatorsDir.entryList(QDir::Files))
    {
        QPluginLoader pluginLoader(embeddedViewCreatorsDir.absoluteFilePath(fileName));
        EmbeddedViewCreatorInterface* viewer = qobject_cast<EmbeddedViewCreatorInterface*>(pluginLoader.instance());
        if (viewer)
        {
            //Found a valid plugin, key all it's supported mimetypes to it
            foreach(QString mimetype, viewer->handledMimeTypes())
            {
                m_embeddedViewCreators[mimetype] = viewer;
            }
        }
    }
}

void Searcher::updateCurrentViewMode()
{
    bool showSearch = true;

    if (m_searchText->text().isEmpty())
        showSearch = false;

    m_fileItemView->setVisible(!showSearch);
    m_searchItemView->setVisible(showSearch);
}

void Searcher::openItem(QString url, QString type, QString name, QIcon icon, bool inBackground)
{
    EmbeddedViewCreatorInterface* viewer = m_embeddedViewCreators.value(type);
    if (viewer)
    {
        QWidget* view = viewer->createViewForFile(url, m_tabbedFileView);
        if (!inBackground)
            m_tabbedFileView->setCurrentIndex(m_tabbedFileView->addTab(view, icon, name));
        else
            m_tabbedFileView->addTab(view, icon, name);
    }
    else
        fprintf(stderr, "No viewer loaded for %s files\n", type.toStdString().c_str());
}

/*void Searcher::searchInBrowser(QString text, bool findNext)
{
    if (!findNext)
    {
        QTextCursor textCursor = m_queryResults->textCursor();
        textCursor.setPosition(textCursor.selectionStart());
        m_queryResults->setTextCursor(textCursor);
    }

    QPalette browserPalette(m_browserSearch->palette());

    if (text.isEmpty())
        browserPalette = QApplication::palette();
    else if (m_queryResults->find(text))
        browserPalette.setColor(QPalette::Base, Qt::darkGreen);
    else
    {
        QTextCursor textCursor = m_queryResults->textCursor();
        textCursor.setPosition(0);
        m_queryResults->setTextCursor(textCursor);
        if (m_queryResults->find(text))
            browserPalette.setColor(QPalette::Base, Qt::darkGreen);
        else
            browserPalette.setColor(QPalette::Base, Qt::darkRed);
    }

    m_browserSearch->setPalette(browserPalette);
}

void Searcher::searchInBrowserNext()
{
    searchInBrowser(m_browserSearch->text(), true);
}*/

Searcher::~Searcher()
{
}

#include "searcher.moc"
