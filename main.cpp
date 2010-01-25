#include <QtGui/QApplication>
#include "Reading.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Reading foo;
    foo.show();
    return app.exec();
}
