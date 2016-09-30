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

    connect(qApp, &QApplication::aboutToQuit, [] {
        qDebug() << "aboutToQuit";
    });
}


Widget::~Widget() {
    qDebug() << "~Widget()";
    delete ui;
}

void Widget::paintEvent(QPaintEvent *) {
    static QPointF points[] = { QPointF(0, 0), QPointF(100, 100), QPointF(200, -100), QPointF(300, 100) };
    QPainterPath path(points[0]);

    for (int i = 0; i < 3; ++i) {
        QPointF startPoint = points[i];
        QPointF endPoint = points[i+1];
        QPointF c1 = QPointF((startPoint.x() + endPoint.x()) / 2, startPoint.y());
        QPointF c2 = QPointF((startPoint.x() + endPoint.x()) / 2, endPoint.y());
        path.cubicTo(c1, c2, endPoint);
    }
//    path.closeSubpath();
//    path.moveTo(points[4]);

    QPainter painter(this);
    painter.setPen(Qt::black);
//    painter.setBrush(Qt::gray);
    painter.translate(10, 200);
    painter.drawPath(path);
}


