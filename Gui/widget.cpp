#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <QListWidget>
#include <QNetworkAccessManager>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, [this] {
        qDebug() << ui->pushButton->pos();
    });
}

Widget::~Widget() {
    delete ui;
}


