#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QThread>
#include <QDir>
#include <QFile>
#include <QFileInfo>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, [this] {
        QDir dir("./a/b");
        if (!dir.exists()) {
            qDebug() << "mkpath";
        dir.mkpath(".");
        }
    });
}


Widget::~Widget() {
    delete ui;
}


