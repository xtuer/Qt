#include "BooksModel.h"
#include "bean/Book.h"
#include <QDebug>

BooksModel::BooksModel(QObject *parent) : QStandardItemModel(parent) {

}

bool BooksModel::canDropMimeData(const QMimeData *data, Qt::DropAction action,
                                 int row, int column,
                                 const QModelIndex &parent) const {
    Q_UNUSED(data);
    Q_UNUSED(action);
    Q_UNUSED(row);
    Q_UNUSED(column);

    // 不允许拖放到教材的节点上
    if (parent.isValid() && parent.data(ROLE_TYPE).toString() == TYPE_BOOK) {
        return false;
    } else {
        return true;
    }
}
