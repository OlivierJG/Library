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
#ifndef ENHANCEDLINEEDIT_H
#define ENHANCEDLINEEDIT_H

#include <QtGui/QLineEdit>

//Horrible hack needed to display that greyed out text when it's empty and without focus
//Can get rid of it with Qt 4.7 which has this feature already
class EnhancedLineEdit : public QLineEdit
{
public:
    EnhancedLineEdit(QString placeHolderText, QWidget* parent = 0);

    virtual QString text();
    virtual void setText(QString text);

protected:
    virtual void focusInEvent(QFocusEvent* event );
    virtual void focusOutEvent(QFocusEvent* event );

private:
    void showPlaceholder(bool show);
    
    QString m_placeholderText;
    QFont m_placeholderFont;
    QPalette m_placeholderPalette;

    QFont m_normalFont;
    QPalette m_normalPalette;

    bool m_showingPlaceholder;
};

#endif // ENHANCEDLINEEDIT_H
