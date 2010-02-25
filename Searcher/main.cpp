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
