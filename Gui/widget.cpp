#include "widget.h"
#include "ui_widget.h"
#include "OxyWidget.h"

#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QTextStream>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    ui->widget->setAttribute(Qt::WA_TransparentForMouseEvents,true);

    connect(ui->pushButton, &QPushButton::clicked, []{
        qDebug() << "clicked";
    });
}

Widget::~Widget() {
    delete ui;
}
