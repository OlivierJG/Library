#include "searchtreeview.h"

SearchTreeView::SearchTreeView(QWidget* parent): QTreeView(parent)
{
    setColumnWidth(1, 300); //Default second column to a more useful width
}

#include "searchtreeview.moc"
