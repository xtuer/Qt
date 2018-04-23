#include "TreeModel.h"
#include <QDebug>
#include <QDataStream>
#include <QMimeData>

TreeModel::TreeModel(QObject *parent) : QStandardItemModel(parent) {

}

//Qt::DropActions TreeModel::supportedDropActions() const {
//    return Qt::MoveAction;
//}

//Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const {
//    Qt::ItemFlags defaultFlags = QStandardItemModel::flags(index) & (~Qt::ItemIsDragEnabled) & (~Qt::ItemIsDropEnabled);

//    //    qDebug() << QString("%1 - %2").arg(defaultFlags, 0, 2).arg(r, 0, 2);

//    if (index.isValid() && index.column() == 0) {
//        //        qDebug() << index.data();
//        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
//    } else if (index.isValid() && index.column() == 1){
//        //        qDebug() << "---" << index.data();
//        return defaultFlags;
//    } else {
//        return defaultFlags;
//    }
//}

//QStringList TreeModel::mimeTypes() const {
//    QStringList types;
//    types << "application/vnd.text.list";
//    return types;
//}

//QMimeData *TreeModel::mimeData(const QModelIndexList &indexes) const {
//    QMimeData *mimeData = new QMimeData();
//    QByteArray encodedData;

//    QDataStream stream(&encodedData, QIODevice::WriteOnly);

//    foreach (const QModelIndex &index, indexes) {
//        if (index.isValid()) {
////            QString text = data(index, Qt::DisplayRole).toString();
//            QString c1 = this->index(index.row(), 0).data().toString();
//            QString c2 = this->index(index.row(), 1).data().toString();
//            stream << c1 << c2;
//            qDebug() << c1 << c2;
//        }
//    }

//    mimeData->setData("application/vnd.text.list", encodedData);
//    return mimeData;
//}

bool TreeModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column,
                                const QModelIndex &parent) const {
    Q_UNUSED(data);
    Q_UNUSED(action);

    qDebug() << QString("Row: %1, Col: %2, Parent: %3, %4").arg(row).arg(column).arg(parent.isValid()).arg(parent.column());
//    if (column > 0)
//        return false;

    if (column > 0) {
        return false;
    } else if (parent.isValid() && parent.column() > 0) {
        return false;
    }

//    if (!data->hasFormat("application/vnd.text.list"))
//        return false;

    return true;
}

//bool TreeModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column,
//                             const QModelIndex &parent) {
//    if (!canDropMimeData(data, action, row, column, parent))
//        return false;

//    if (action == Qt::IgnoreAction)
//        return true;

//    int beginRow;

//    if (row != -1)
//        beginRow = row;
//    else if (parent.isValid())
//        beginRow = parent.row();
//    else
//        beginRow = rowCount(QModelIndex());

//    qDebug() << parent << parent.data();

//    QByteArray encodedData = data->data("application/vnd.text.list");
//    QDataStream stream(&encodedData, QIODevice::ReadOnly);
//    QStringList newItems;
//    int rows = 0;

//    while (!stream.atEnd()) {
//        QString text;
//        stream >> text;
//        newItems << text;
//        ++rows;
//    }

//    insertRows(beginRow, rows/2, QModelIndex());

//    for (int i = 0; i < newItems.size(); i+=2) {
//        QModelIndex idx1 = index(beginRow, 0, QModelIndex());
//        setData(idx1, newItems[i]);
//        QModelIndex idx2 = index(beginRow, 1, QModelIndex());
//        setData(idx2, newItems[i+1]);
//        beginRow++;
//    }
////    foreach (const QString &text, newItems) {
////        QModelIndex idx1 = index(beginRow, 0, QModelIndex());
////        setData(idx1, text);
////        QModelIndex idx2 = index(beginRow, 0, QModelIndex());
////        beginRow++;
////    }

//    return true;
//}


bool TreeModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column,
                             const QModelIndex &parent) {
    qDebug() << parent.row() << parent.column();
    return QStandardItemModel::dropMimeData(data, action, row, column, parent);

//    if (column == 0) {
//        return QStandardItemModel::dropMimeData(data, action, row, column, parent);
//    } else {
//        return false;
//    }
}





