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
#include <QHBoxLayout>
#include <QScrollBar>

Searcher::Searcher()
{
    m_libPath = "../../Library/";

    resize(500,500);

    m_searchModel = new SearchModel("../../.libindex/", m_libPath, this);

    m_fileSystemModel = new QFileSystemModel(this);
    m_fileSystemModel->setRootPath(m_libPath);
    m_fileSystemModel->setNameFilterDisables(false);


    setCentralWidget(new QWidget);
    m_filterText = new MyLineEdit("Filter...", centralWidget());
    m_searchText = new MyLineEdit("Search...", centralWidget());
    m_contextInfo = new QLabel(centralWidget());
    m_fileSystemTree = new QTreeView(centralWidget());
    m_fileSystemTree->setModel(m_fileSystemModel);
    m_fileSystemTree->setRootIndex(m_fileSystemModel->index(m_libPath));
    m_fileSystemTree->setColumnWidth(0, 300);
    m_searchResultsTreeView = new SearchTreeView(centralWidget());
    m_searchResultsTreeView->setModel(m_searchModel);
    m_searchResultsTreeView->setVisible(false);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(m_contextInfo);
    layout->addWidget(m_fileSystemTree);
    layout->addWidget(m_searchResultsTreeView);
    layout->addWidget(m_filterText);
    layout->addWidget(m_searchText);
    centralWidget()->setLayout(layout);

    connect(m_searchText, SIGNAL(textChanged(QString)), this, SLOT(searchTextChanged(QString)));
    connect(m_filterText, SIGNAL(textChanged(QString)), this, SLOT(filterTextChanged(QString)));
    connect(m_searchModel, SIGNAL(rowsInserted(const QModelIndex,int,int)), this, SLOT(updateContextInfo()));
    connect(m_searchModel, SIGNAL(rowsRemoved(const QModelIndex,int,int)), this, SLOT(updateContextInfo()));

    updateContextInfo();
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
    if (m_searchResultsTreeView->isVisible())
        m_contextInfo->setText(QString::number(m_searchModel->rowCount()) +
                               " result" +
                               (m_searchModel->rowCount() == 1 ? " from" : "s from") +
                               m_libPath);
    else
        m_contextInfo->setText("Browsing Library in " + m_libPath);
}

void Searcher::updateCurrentViewMode()
{
    bool showSearch = true;

    if (m_searchText->text().isEmpty())
        showSearch = false;

    m_fileSystemTree->setVisible(!showSearch);
    m_searchResultsTreeView->setVisible(showSearch);
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
