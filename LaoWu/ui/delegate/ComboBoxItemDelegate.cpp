#include "ComboBoxItemDelegate.h"

#include <QComboBox>
#include <QStringList>

struct ComboBoxItemDelegatePrivate {
    QStringList items;
};

ComboBoxItemDelegate::ComboBoxItemDelegate(QStringList &items, QObject *parent) :
    QItemDelegate(parent) {

    data = new ComboBoxItemDelegatePrivate;
    data->items = items;
}

ComboBoxItemDelegate::~ComboBoxItemDelegate() {
    delete data;
}

QWidget *ComboBoxItemDelegate::createEditor(QWidget *parent,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const {
    Q_UNUSED(option);
    Q_UNUSED(index);

    QComboBox *combo = new QComboBox(parent);
    combo->setEditable(true);
    combo->addItems(data->items);

    return combo;
}

void ComboBoxItemDelegate::setEditorData(QWidget *editor,
                                         const QModelIndex &index) const {
     QComboBox *combo = qobject_cast<QComboBox*>(editor);
     QString text = index.data(Qt::EditRole).toString();
     combo->setCurrentText(text);
}

void ComboBoxItemDelegate::setModelData(QWidget *editor,
                                        QAbstractItemModel *model,
                                        const QModelIndex &index) const {
    QComboBox *combo = qobject_cast<QComboBox*>(editor);
    QString text = combo->currentText();
    model->setData(index, text);
}
