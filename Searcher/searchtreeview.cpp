#include "searchtreeview.h"

SearchTreeView::SearchTreeView(QWidget* parent): QTreeView(parent)
{
    setAttribute(Qt::WA_PaintOnScreen);
}

void SearchTreeView::setModel(QAbstractItemModel* model)
{
    QTreeView::setModel(model);
    setColumnWidth(1, 300); //Default second column to a more useful width
}

#include "searchtreeview.moc"
