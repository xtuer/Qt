#ifndef LINEEDITDELEGATE_H
#define LINEEDITDELEGATE_H

#include <QStyledItemDelegate>

class LineEditDelegate : public QStyledItemDelegate {
public:
    LineEditDelegate(QObject *parent = nullptr);

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
};

#endif // LINEEDITDELEGATE_H
