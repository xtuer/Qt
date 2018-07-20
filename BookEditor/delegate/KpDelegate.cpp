#include "KpDelegate.h"
#include "gui/TopWindow.h"
#include "gui/CognitionWidget.h"
#include "service/Service.h"

#include <QDebug>
#include <QModelIndex>
#include <QStandardItemModel>

KpDelegate::KpDelegate(QItemSelectionModel *subjectsSelectionModel, QObject *parent)
    : QStyledItemDelegate(parent), subjectsSelectionModel(subjectsSelectionModel) {
    cognitionWidget = new CognitionWidget();
    topWindow = new TopWindow(cognitionWidget);
    topWindow->setTitleBarButtonsVisible(false, false, true);
    topWindow->resize(660, 430);
}

KpDelegate::~KpDelegate() {
    delete topWindow;
}

QWidget *KpDelegate::createEditor(QWidget *parent,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const {
    // 1. 如果是第 2 列或者第 3 列，说明要编辑的是认知水平，弹出认知水平选择对话框进行选择
    //    1.1 如果没有选择学科则返回
    //    1.2 获取学段和学科，然后显示对应的认知水平
    //    1.3 获取选择的认知水平，设置到当前单元格
    // 2. 如果是第 4 列或者第 5 列，说明要编辑的是学业质量，弹出学业质量选择对话框进行选择

    // 获取学段和学科
    QModelIndex subjectIndex = subjectsSelectionModel->selectedIndexes().value(0);
    QString subjectName = subjectIndex.data().toString();
    QString phaseName   = subjectIndex.parent().data().toString();

    if (index.column() == 2 || index.column() == 3) {
        // [1] 如果是第 2 列或者第 3 列，说明要编辑的是认知水平，弹出认知水平选择对话框进行选择
        // [1.1] 如果没有选中学科则返回
        if (!Service::isSubjectIndex(subjectIndex)) { return nullptr; }

        // [1.2] 获取学段和学科，然后显示对应的认知水平
        cognitionWidget->setPhaseNameAndSubjectName(phaseName, subjectName);
        topWindow->showModal();

        // [1.3] 获取选择的认知水平，设置到当前单元格
        if (cognitionWidget->isSelected()) {
            QAbstractItemModel *model = const_cast<QAbstractItemModel*>(index.model());
            model->setData(index, cognitionWidget->getSelectedCognitions());
        }

        return nullptr;
    } else if (index.column() == 4 || index.column() == 5) {
        // [2] 如果是第 4 列或者第 5 列，说明要编辑的是学业质量，弹出学业质量选择对话框进行选择
        return nullptr;
    } else {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

void KpDelegate::updateEditorGeometry(QWidget *editor,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const {
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}
