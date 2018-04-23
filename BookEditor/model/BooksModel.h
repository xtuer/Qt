#ifndef BOOKSMODEL_H
#define BOOKSMODEL_H

#include <QStandardItemModel>

/**
 * 教材结构树的 model，主要是实现控制拖拽操作
 */
class BooksModel : public QStandardItemModel {
public:
    BooksModel(QObject *parent = 0);
    bool canDropMimeData(const QMimeData *data, Qt::DropAction action,
                         int row, int column,
                         const QModelIndex &parent) const override;
};

#endif // BOOKSMODEL_H
