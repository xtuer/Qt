#ifndef SORTMODEL_H
#define SORTMODEL_H

#include <QSortFilterProxyModel>
#include <QHash>
#include <QColor>

class SortModel : public QSortFilterProxyModel {
public:
    SortModel(QObject *parent = NULL);

protected:
    virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
    QHash<QString, QColor> colors;
};

#endif // SORTMODEL_H
