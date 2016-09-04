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
#include <QGraphicsScene>
#include <QFont>
#include <QSplitter>
#include <QResizeEvent>
#include <QRubberBand>
#include <QDateTime>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    //    ui->widget->setAttribute(Qt::WA_TransparentForMouseEvents,true);

    count = 0;

    manager = new QNetworkAccessManager(this);

    connect(ui->pushButton, &QPushButton::clicked, [this]() {
        for (int i = 0; i < 1; ++i) {
            accessUrl();
        }
    });

    connect(manager, &QNetworkAccessManager::finished, [this](QNetworkReply *reply) {
        qDebug() << "-----------" << reply->readBufferSize();
        QString result = reply->readAll();
        qDebug() << result;

//        if (count < 300) {
//            accessUrl();
//            ++count;
//        }
    });

}

Widget::~Widget() {
    delete ui;
}

void Widget::resizeEvent(QResizeEvent *event) {

}

void Widget::accessUrl() {
    qint64 ms = QDateTime::currentDateTime().toMSecsSinceEpoch();
    QString url = "http://i.edu-edu.com.cn/sale/public/comments/counter/add/1/2684/1/jsonp/__sale_product_counter_loaded?_=";
    //this->manager->get(QNetworkRequest(QUrl(QString("%1%2").arg(url).arg(ms))));

    url = "http://www.xtuer-git.com";
    this->manager->get(QNetworkRequest(QUrl(url)));
}
