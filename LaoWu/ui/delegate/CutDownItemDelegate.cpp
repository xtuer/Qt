#include "CutDownItemDelegate.h"
#include "../../util/Global.h"
#include <QPainter>

CutDownItemDelegate::CutDownItemDelegate(QObject *parent) : QItemDelegate(parent) {
}

void CutDownItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    painter->fillRect(option.rect, QBrush(QColor(200, 200, 200)));
    QItemDelegate::paint(painter, option, index);
}
