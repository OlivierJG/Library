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

#ifndef TEXTFILTERINTERFACE_H
#define TEXTFILTERINTERFACE_H

#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <string>

using namespace std;

class TextFilterInterface
{
    public:
        virtual ~TextFilterInterface() {}
        virtual QStringList handledMimeTypes() = 0;
        virtual string textForFile(QString filePath) = 0;
};

Q_DECLARE_INTERFACE(TextFilterInterface, "app.indexer.Plugin.TextFilterInterface/1.0");

#endif // TEXTFILTERINTERFACE_H
