#include "KpDelegate.h"
#include "gui/MessageBox.h"
#include <QDebug>
#include <QModelIndex>
#include <QStandardItemModel>

KpDelegate::KpDelegate(QObject *parent) : QStyledItemDelegate(parent) {

}

QWidget *KpDelegate::createEditor(QWidget *parent,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const {
    // 弹出认知发展的选择对话框
    if (index.column() == 2 || index.column() == 3) {
        bool b = MessageBox::confirm("Hi");
        (const_cast<QAbstractItemModel*>(index.model()))->setData(index, b);
        return 0;
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void KpDelegate::updateEditorGeometry(QWidget *editor,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const {
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}
