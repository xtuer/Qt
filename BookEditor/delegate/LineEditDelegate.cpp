#include "LineEditDelegate.h"

LineEditDelegate::LineEditDelegate(QObject *parent) : QStyledItemDelegate(parent) {

}

void LineEditDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const {
    editor->setGeometry(option.rect);
}
