#include <QtGui/QApplication>
#include <xapian.h>
#include <iostream>

#include "searcher.h"

using namespace std;

int main(int argc, char** argv)
{
    try {
        QApplication app(argc, argv);
        Searcher searcher;
        searcher.show();
        return app.exec();
    }
    catch (const Xapian::Error & error) {
        cout << "Xapian Exception: " << error.get_msg() << endl;
    }
}
