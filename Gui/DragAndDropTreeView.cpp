#include "ui_DragAndDropTreeView.h"
#include "DragAndDropTreeView.h"
#include "TreeModel.h"

#include <QStandardItem>

DragAndDropTreeView::DragAndDropTreeView(QWidget *parent) : QWidget(parent), ui(new Ui::DragAndDropTreeView) {
    ui->setupUi(this);
    model = new TreeModel();
    ui->treeView->setModel(model);

    for (int i = 0; i < 4; ++i) {
        QStandardItem *item = new QStandardItem(QString("外面 %1").arg(i));

        for (int j = 0; j < 3; ++j) {
            QList<QStandardItem *> columns;
            columns << new QStandardItem(QString("第一列 %1-%2").arg(i).arg(j + (i+1)*100))
                    << new QStandardItem(QString("第二列 %1-%2").arg(i).arg(j + (i+1)*200));
            item->appendRow(columns);
        }

        QList<QStandardItem *> columns2;
        columns2 << item << new QStandardItem(QString("Item-L %1").arg(i + 300));
        model->appendRow(columns2);
    }

    ui->treeView->setDragEnabled(true);
    ui->treeView->setAcceptDrops(true);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeView->setDropIndicatorShown(true);
    ui->treeView->setDragDropMode(QAbstractItemView::InternalMove);
    ui->treeView->setColumnWidth(0, 200);
    ui->treeView->expandAll();
}

DragAndDropTreeView::~DragAndDropTreeView() {
    delete ui;
}
