#include "widget.h"
#include "ui_widget.h"
#include "Bobo.h"

#include <QDebug>
#include <QListWidget>
#include <QNetworkAccessManager>
#include "HttpClient.h"
#include <QFile>
#include <QDataStream>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    ui->scrollArea->setContentsMargins(0, 0, 0, 0);
    ui->scrollArea->widget()->setContentsMargins(0, 0, 0, 0);

    ui->scrollArea->widget()->layout()->setContentsMargins(0, 0, 0, 0);
    qDebug() << ui->scrollArea->widget()->objectName();

    connect(ui->pushButton, &QPushButton::clicked, [this] {
        qDebug() << ui->scrollArea->widget()->size();
        qDebug() << ui->scrollArea->viewport()->size();
    });
}


Widget::~Widget() {
    delete ui;
}


