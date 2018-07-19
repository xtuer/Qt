#ifndef KPDELEGATE_H
#define KPDELEGATE_H

#include <QStyledItemDelegate>

/**
 * 知识点的代理
 */
class KpDelegate : public QStyledItemDelegate {
public:
    KpDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
};

#endif // KPDELEGATE_H
