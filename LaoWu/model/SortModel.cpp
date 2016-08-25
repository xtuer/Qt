#include "SortModel.h"
#include "../util/Global.h"
#include <QColor>
#include <QList>
#include <QDebug>

SortModel::SortModel(QObject *parent) : QSortFilterProxyModel(parent) {
    colors["<=0"] = QColor(255, 0, 0);
    colors["1<<=3"] = QColor(0, 0, 255);
    // 股票 紫色，期货绿色， 民贷黄色，基金黑色
    colors[QString("股票")] = QColor(Qt::magenta);
    colors[QString("期货")] = QColor(Qt::darkGreen);
    colors[QString("民贷")] = QColor(Qt::darkCyan);
    colors[QString("基金")] = QColor(0, 0, 0);
}

bool SortModel::lessThan(const QModelIndex &left, const QModelIndex &right) const {
    // The index left is the source model's index.
    // if (left.column() >= 0 && left.column() <= 3) {
    if (left.column() == 0) {
        int topRow = left.row();
        QString top0 = sourceModel()->data(sourceModel()->index(topRow, 0)).toString();
        QString top1 = sourceModel()->data(sourceModel()->index(topRow, 1)).toString();
        QString top2 = sourceModel()->data(sourceModel()->index(topRow, 2)).toString();
        QString top3 = sourceModel()->data(sourceModel()->index(topRow, 3)).toString();

        int bottomRow = right.row();
        QString bottom0 = sourceModel()->data(sourceModel()->index(bottomRow, 0)).toString();
        QString bottom1 = sourceModel()->data(sourceModel()->index(bottomRow, 1)).toString();
        QString bottom2 = sourceModel()->data(sourceModel()->index(bottomRow, 2)).toString();
        QString bottom3 = sourceModel()->data(sourceModel()->index(bottomRow, 3)).toString();

        if (top0 == bottom0) {
            if (top1 == bottom1) {
                if (top2 == bottom2) {
                    return top3 < bottom3;
                } else {
                    return top2 < bottom2;
                }
            } else {
                return top1 < bottom1;
            }
        } else {
            return top0 < bottom0;
        }
    } else {
        return QSortFilterProxyModel::lessThan(left, right);
    }
}

QVariant SortModel::data(const QModelIndex &index, int role) const {
    // Can not directly fetch data from the above index.data().
    QModelIndex sourceIndex = mapToSource(index);
    int days = Global::stringToInt(sourceModel()->data(sourceModel()->index(sourceIndex.row(), 7)).toString(), tr("天"));
    QString leiXin = sourceModel()->data(sourceModel()->index(sourceIndex.row(), 0)).toString();

    if ((role == Qt::ForegroundRole)) {
        if (days <= 0) {
            return colors["<=0"];
        } else if (0 < days && days <= 3) {
            return colors["1<<=3"];
        } else {
            foreach (QString lx, colors.keys()) {
                if (lx == leiXin) {
                    return colors[lx];
                }
            }
        }
    }

    return QSortFilterProxyModel::data(index, role);
}
