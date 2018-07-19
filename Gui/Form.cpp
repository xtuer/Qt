#include "Form.h"
#include "ui_Form.h"

#include <QDebug>
#include <QImage>
#include <QMatrix>
#include <QPixmap>
#include <QPainter>
#include <QList>
#include <QStandardItem>

Form::Form(QWidget *parent) : QWidget(parent), ui(new Ui::Form) {
    ui->setupUi(this);

    model = new QStandardItemModel(this);
    for (int i = 0; i < 5; i++) {
        QList<QStandardItem*> items;

        for (int j = 0; j < 5; j++) {
            items << new QStandardItem(QString("%1-%2").arg(i).arg(j));
        }

        model->appendRow(items);
    }

    ui->tableView->setModel(model);
//    ui->tableView->horizontalHeader()->hideSection(1);
    ui->tableView->horizontalHeader()->setSortIndicatorShown(true);

    connect(ui->pushButton, &QPushButton::clicked, [=] {
//        qDebug() << ui->tableView->horizontalHeader()->visualIndex(0)
//                 << ui->tableView->horizontalHeader()->visualIndex(1)
//                 << ui->tableView->horizontalHeader()->visualIndex(2)
//                 << ui->tableView->horizontalHeader()->visualIndex(3);
//        qDebug() << ui->tableView->horizontalHeader()->isSectionHidden(0)
//                 << ui->tableView->horizontalHeader()->isSectionHidden(1);
        ui->tableView->horizontalHeader()->swapSections(0, 4);

    });
}

void Form::paintEvent(QPaintEvent *event) {

}
