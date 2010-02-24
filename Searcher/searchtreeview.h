#ifndef SEARCHTREEVIEW_H
#define SEARCHTREEVIEW_H

#include <QtGui/QTreeView>
#include <QApplication>
#include <QEvent>


class SearchTreeView : public QTreeView
{
public:
    virtual void setModel(QAbstractItemModel* model);
    SearchTreeView(QWidget* parent = 0);
};

#endif // SEARCHTREEVIEW_H
