#include "LogHandler.h"

#include <QApplication>
#include <QPushButton>
#include <QDebug>
#include <QDateTime>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Singleton<LogHandler>::getInstance().installMessageHandler();

    QPushButton *button = new QPushButton("点击我");
    button->show();

    QObject::connect(button, &QPushButton::clicked, [] {
        qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    });

    qDebug() << "Hello";
    qDebug() << "你好";
    qDebug() << "哪里";
    qDebug() << "Hello";
    qInfo() << QString("God");


    int ret = a.exec();
    Singleton<LogHandler>::getInstance().release();

    return ret;
}
