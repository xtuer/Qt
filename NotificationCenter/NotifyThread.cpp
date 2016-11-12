#include "NotifyThread.h"
#include "NotificationCenter.h"
#include <QDebug>
#include <QDateTime>

NotifyThread::NotifyThread() {}

void NotifyThread::run() {
    qDebug() << "NotifyThread: " << QThread::currentThread();
    qsrand(QDateTime::currentDateTime().toMSecsSinceEpoch());

    while (true) {
        NOTIFY_CROSS_THREAD(4, QString::number(qrand() % 900 + 100).toUtf8());

        QThread::msleep(1000);
    }
}
