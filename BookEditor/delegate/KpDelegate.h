#ifndef KPDELEGATE_H
#define KPDELEGATE_H

#include <QStyledItemDelegate>
#include <QItemSelectionModel>

class TopWindow;
class CognitionWidget;

/**
 * 知识点的代理
 */
class KpDelegate : public QStyledItemDelegate {
public:
    KpDelegate(QItemSelectionModel *subjectsSelectionModel, QObject *parent = nullptr);
    ~KpDelegate() override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

private:
    TopWindow *topWindow = nullptr;
    CognitionWidget *cognitionWidget = nullptr;
    QItemSelectionModel *subjectsSelectionModel;
};

#endif // KPDELEGATE_H
