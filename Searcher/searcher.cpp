#include "searcher.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QMessageBox>

Searcher::Searcher()
{
    try {
        initXapian();
    }
    catch (const Xapian::Error & error) {
        cout << "Xapian Exception: " << error.get_msg() << endl;
    }
    catch (const SearchError & error) {
        cout << error.message.toLatin1().data() << endl;
    }

    QWidget *window = new QWidget;
    setCentralWidget(window);

    m_searchText = new QLineEdit(centralWidget());
    m_queryResults = new QueryResultView("/home/louis/Projects/Reading/Library", centralWidget());
    m_searchInfo = new QLineEdit(centralWidget());
    m_searchInfo->setReadOnly(true);
    m_browserSearch = new QLineEdit(centralWidget());

    QVBoxLayout* layout = new QVBoxLayout();
    QHBoxLayout* hlayout = new QHBoxLayout();
    hlayout->addWidget(m_searchText);
    hlayout->addWidget(m_searchInfo);
    layout->addLayout(hlayout);
    layout->addWidget(m_queryResults);
    layout->addWidget(m_browserSearch);
    centralWidget()->setLayout(layout);

    connect(m_searchText, SIGNAL(textChanged(QString)), this, SLOT(updateSearchQuery(QString)));
    connect(m_queryResults, SIGNAL(scrolledToEnd()), this, SLOT(getMoreQueryResults()));
    connect(m_queryResults, SIGNAL(resized()), this, SLOT(fillResultsWidget()));
    connect(m_browserSearch, SIGNAL(textChanged(QString)), this, SLOT(searchInBrowser(QString)));
    connect(m_browserSearch, SIGNAL(returnPressed()), this, SLOT(searchInBrowserNext()));
}

void Searcher::initXapian()
{
    QString dbPath;
    if (QApplication::arguments().size() > 1)
        dbPath = QApplication::arguments()[1];
    else
        throw SearchError("Requires path to database as argument", this);

    m_xapianDb = new Xapian::Database(dbPath.toStdString());
    m_xapianEnquire = new Xapian::Enquire(*m_xapianDb);
    m_xapianQueryParser = new Xapian::QueryParser();
}

void Searcher::updateSearchQuery(QString query)
{
    Xapian::Query xapianQuery = m_xapianQueryParser->parse_query(query.toStdString());

    m_xapianEnquire->set_query(xapianQuery);

    m_queryResults->clear();
    fillResultsWidget();
}

void Searcher::updateSearchInfo()
{
    m_searchInfo->setText("Showing " +
                          QString::number(m_queryResults->resultCount()) +
                          " of approximately " +
                          QString::number(m_xapianMatchSet.get_matches_estimated()) +
                          (m_xapianMatchSet.get_matches_estimated() == 1 ? " result" : " results"));
}

void Searcher::getMoreQueryResults()
{
    m_xapianMatchSet = m_xapianEnquire->get_mset(m_queryResults->resultCount(), 1);

    Xapian::MSetIterator i;
    for (i = m_xapianMatchSet.begin(); i != m_xapianMatchSet.end(); ++i) {
        Xapian::Document doc = i.get_document();
        m_queryResults->addResult(doc.get_data().c_str(), i.get_percent());
    }

    updateSearchInfo();
}

void Searcher::fillResultsWidget()
{
    int currentResults = 0;
    while (m_queryResults->document()->size().height() < m_queryResults->contentsRect().height())
    {
        getMoreQueryResults();
        if (currentResults == m_queryResults->resultCount())
            break;
        else
            currentResults = m_queryResults->resultCount();
    }
}

void Searcher::searchInBrowser(QString text, bool findNext)
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
}

Searcher::~Searcher()
{
    if (m_xapianDb)
        delete m_xapianDb;
    
    if (m_xapianEnquire)
        delete m_xapianEnquire;

    if (m_xapianQueryParser)
        delete m_xapianQueryParser;
}

#include "searcher.moc"
