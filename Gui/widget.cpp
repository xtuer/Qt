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
#include <QPaintEvent>
#include <QPainterPath>
#include <QPainter>
#include <QApplication>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
}


Widget::~Widget() {
    qDebug() << "~Widget()";
    delete ui;
}



