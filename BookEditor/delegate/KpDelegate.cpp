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
    // 弹出认知发展的选择对话框
    if (index.column() == 2 || index.column() == 3) {
        QModelIndexList indexes = subjectsSelectionModel->selectedIndexes();

        if (indexes.size() == 0) { return 0; }
        if (!Service::isSubjectIndex(indexes.at(0))) { return 0; }

        QString subjectName = indexes.at(0).data().toString();
        QString phaseName = indexes.at(0).parent().data().toString();
        cognitionWidget->setPhaseNameAndSubjectName(phaseName, subjectName);
        topWindow->showModal();

        if (cognitionWidget->isSelected()) {
            QAbstractItemModel *model = const_cast<QAbstractItemModel*>(index.model());
            model->setData(index, cognitionWidget->getSelectedCognitions());
        }

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
