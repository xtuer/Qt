#ifndef CUTDOWNITEMDELEGATE_H
#define CUTDOWNITEMDELEGATE_H

#include <QItemDelegate>

class CutDownItemDelegate : public QItemDelegate {
public:
    CutDownItemDelegate(QObject *parent = NULL);

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // CUTDOWNITEMDELEGATE_H
