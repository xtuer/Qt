#include "ui_DragAndDropTreeView.h"
#include "DragAndDropTreeView.h"
#include <QStandardItem>

DragAndDropTreeView::DragAndDropTreeView(QWidget *parent) : QWidget(parent), ui(new Ui::DragAndDropTreeView) {
    ui->setupUi(this);
    model = new QStandardItemModel();
    ui->treeView->setModel(model);

    for (int i = 0; i < 10; ++i) {
        QStandardItem *item = new QStandardItem(QString("Item I %1").arg(i));

        for (int j = 0; j < 3; ++j) {
            QList<QStandardItem *> columns;
            columns << new QStandardItem(QString("Item-J %1").arg(j + 100)) << new QStandardItem(QString("Item-K %1").arg(j + 200));
            item->appendRow(columns);
        }

        QList<QStandardItem *> columns2;
        columns2 << item << new QStandardItem(QString("Item-L %1").arg(i + 300));
        model->appendRow(columns2);
    }

//    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeView->setDragEnabled(true);
    ui->treeView->viewport()->setAcceptDrops(true);
    ui->treeView->setDropIndicatorShown(true);
    ui->treeView->setDragDropMode(QAbstractItemView::InternalMove);
    ui->treeView->header()->setStretchLastSection(true);
}

DragAndDropTreeView::~DragAndDropTreeView() {
    delete ui;
}
