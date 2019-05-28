#include "MyWidget.h"
#include "ui_MyWidget.h"
#include "MessageShower.h"

#include <QDebug>
#include <QDateTime>
#include <QTimer>

#include <QTableView>
#include <QModelIndex>
#include <QItemSelectionModel>

MyWidget::MyWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MyWidget) {
    initializeUi();
    handleEvents();

    ui->tableWidget->setCurrentCell(2, 0);
}

MyWidget::~MyWidget() {
    qDebug() << "~MyWidget()";
    delete ui;
}

void MyWidget::initializeUi() {
    ui->setupUi(this);
    ui->tableWidget->hideRow(2);
}

void MyWidget::handleEvents() {
    QList<int> ns = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
    QList<int> iv = { 0, 2, 5, 8 };

    for (int n : iv) {
        ns.removeOne(n);
    }

    qDebug() << ns << ns.first();

    connect(ui->tableWidget, &QTableWidget::currentCellChanged, [this] (int row) {
        qDebug() << row << ", " << ui->tableWidget->currentItem()->data(Qt::DisplayRole).toString();
    });
}

