#include "XThread.h"
#include <QDateTime>
#include <QDebug>

XThread::XThread() {
}

void XThread::run() {
    qDebug() << "XThread:  " << QThread::currentThread();

    while (true) {
        emit currentTime(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
        QThread::msleep(1000);
    }
}
