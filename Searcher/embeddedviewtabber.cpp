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
#include <iostream>
#include "embeddedviewtabber.h"
#include <qstyle.h>
#include <qstyleoption.h>
#include "embeddedviewcreatorinterface.h"

EmbeddedViewTabber::EmbeddedViewTabber(QWidget* parent): QTabWidget(parent)
{
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    setDocumentMode(true);
    setTabsClosable(true);
    setElideMode(Qt::ElideRight);
    style()->styleHint(QStyle::SH_TabBar_PreferNoArrows);
}

void EmbeddedViewTabber::closeTab(int index)
{
    QWidget* view = widget(index);
    removeTab(index);
    delete view;
}

void EmbeddedViewTabber::keyPressEvent(QKeyEvent* keyEvent)
{
    if (keyEvent->key() == Qt::Key_W && keyEvent->modifiers() == Qt::ControlModifier)
    {
        closeTab(currentIndex());
        if (currentWidget())
            currentWidget()->setFocus();
    }

    QTabWidget::keyPressEvent(keyEvent);
}

#include "embeddedviewtabber.moc"
