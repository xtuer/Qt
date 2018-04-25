#include "RegionModel.h"

RegionModel::RegionModel(QObject *parent) : QStandardItemModel(parent) {
}

/**
 * 返回 true 允许拖放，返回 false 禁止拖放
 */
bool RegionModel::canDropMimeData(const QMimeData *data,
                                  Qt::DropAction action,
                                  int row, int column,
                                  const QModelIndex &parent) const {
    Q_UNUSED(data);
    Q_UNUSED(action);
    Q_UNUSED(row);

    // 只允许拖放到第 0 列
    if (column > 0) {
        return false;
    } else if (parent.isValid() && parent.column() > 0) {
        return false;
    } else {
        return true;
    }
}
