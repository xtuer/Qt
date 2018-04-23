#ifndef CHAPTERSMODEL_H
#define CHAPTERSMODEL_H

#include <QStandardItemModel>

/**
 * 章节目录树的 model，主要是实现控制拖拽操作
 */
class ChaptersModel : public QStandardItemModel {
public:
    ChaptersModel(QObject *parent = 0);
    bool canDropMimeData(const QMimeData *data, Qt::DropAction action,
                         int row, int column,
                         const QModelIndex &parent) const override;
};

#endif // CHAPTERSMODEL_H
