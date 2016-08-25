#ifndef COMBOBOXITEMDELEGATE_H
#define COMBOBOXITEMDELEGATE_H

#include <QItemDelegate>

class QStringList;
struct ComboBoxItemDelegatePrivate;

class ComboBoxItemDelegate : public QItemDelegate {
public:
    explicit ComboBoxItemDelegate(QStringList &items, QObject *parent = NULL);
    ~ComboBoxItemDelegate();

protected:
    virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private:
    ComboBoxItemDelegatePrivate *data;
};

#endif // COMBOBOXITEMDELEGATE_H
