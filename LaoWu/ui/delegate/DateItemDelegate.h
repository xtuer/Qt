#ifndef DATEITEMDELEGATE_H
#define DATEITEMDELEGATE_H

#include <QItemDelegate>

class QDate;
struct DateItemDelegatePrivate;

class DateItemDelegate : public QItemDelegate {
public:
    explicit DateItemDelegate(const QDate &defaultDate, QObject *parent = 0);
    ~DateItemDelegate();

protected:
    virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private:
    DateItemDelegatePrivate *data;
};

#endif // DATEITEMDELEGATE_H
