#include <QtGui/QApplication>
#include "searcher.h"
#include <xapian.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Searcher searcher;
    searcher.show();
    return app.exec();
}
