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

    Bobo *x = new Bobo;
    QFile *file = new QFile("/Users/Biao/Desktop/bii.png");
    file->open(QIODevice::WriteOnly);

    connect(ui->pushButton, &QPushButton::clicked, [x] {
        HttpClient("http://gamerboom.com/wp-content/uploads/2010/11/Mac-App-Store.png").download([x](const QByteArray &data) {
            x->name = QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());
            qDebug() << x->name;
        }, [x] {
            qDebug() << "Finish: " << x->name;
            delete x;
        });
    });
}


Widget::~Widget() {
    delete ui;
}


