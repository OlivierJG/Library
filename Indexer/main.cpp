#include <QtCore/QCoreApplication>
#include <iostream>

#include "indexer.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    if (argc == 2)
    {
        Indexer indexer(argv[1]);
        return static_cast<int>(indexer.hasStopError());
    }
    else
        cout << "Usage: indexer /path/to/dir/for/indexing" << endl;

    return 0;
}
