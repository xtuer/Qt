#ifndef REGIONMODEL_H
#define REGIONMODEL_H

#include <QStandardItemModel>

class RegionModel : public QStandardItemModel {
public:
    RegionModel(QObject *parent = 0);
    bool canDropMimeData(const QMimeData *data, Qt::DropAction action,
                         int row, int column,
                         const QModelIndex &parent) const override;
};

#endif // REGIONMODEL_H
