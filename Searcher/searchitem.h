#ifndef SEARCHITEM_H
#define SEARCHITEM_H

#include <QString>
#include <QIcon>

struct SearchItem
{
    SearchItem() : url(""), score(0), size(""), modified(""), type(""), icon() { }
    SearchItem(SearchItem* other)
    : url(other->url), score(other->score),
      size(other->size), modified(other->modified),
      type(other->type), icon(other->icon) { }
    QString url;
    int score;
    QString size;
    QString modified;
    QString type;
    QIcon icon;

    bool isValid()
    {
        return (!url.isEmpty());
    }
};

#endif