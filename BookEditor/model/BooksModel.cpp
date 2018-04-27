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

    if (false) {
        // parent.isValid() 为 false 时说明拖拽到了 root 上，此时 row, column 为 -1
        qDebug() << QString("Current: [%1, %2], Parent: [%3, %4]-%5 - %6")
                    .arg(row).arg(column)
                    .arg(parent.row()).arg(parent.column())
                    .arg(parent.data().toString()).arg(parent.isValid() ? "true" : "false");
    }

    if (!draggedIndex.isValid()) {
        return false;
    }

    QString parentType  = parent.data(ROLE_TYPE).toString();
    QString currentType = draggedIndex.data(ROLE_TYPE).toString();

    // 1. 阶段只允许拖拽到根节点上
    // 2. 学科只允许拖拽到阶段上
    // 3. 版本只允许拖拽到学科上
    // 4. 教材只允许拖拽到版本上

    if (TYPE_PHASE == currentType && !parent.isValid()) {
        return true;
    } else if (TYPE_SUBJECT == currentType && TYPE_PHASE == parentType) {
        return true;
    } else if (TYPE_VERSION == currentType && TYPE_SUBJECT == parentType) {
        return true;
    } else if (TYPE_BOOK == currentType && TYPE_VERSION == parentType) {
        return true;
    } else {
        return false;
    }
}

QMimeData *BooksModel::mimeData(const QModelIndexList &indexes) const {
    // 记住被拖拽的 index
    BooksModel *self = const_cast<BooksModel*>(this);
    self->draggedIndex = indexes.size() > 0 ? indexes[0] : QModelIndex();

    return QStandardItemModel::mimeData(indexes);
}
