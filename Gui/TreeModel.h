#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QStandardItemModel>

class TreeModel : public QStandardItemModel {
public:
    TreeModel(QObject *parent = 0);
//    Qt::DropActions supportedDropActions() const override;
//    Qt::ItemFlags flags(const QModelIndex &index) const override;
//    QStringList mimeTypes() const override;
//    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column,
                         const QModelIndex &parent) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column,
                      const QModelIndex &parent) override;
};

#endif // TREEMODEL_H
