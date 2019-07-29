#include "Thread.h"
#include <QDateTime>
#include <QDebug>
#include <QTimer>

Thread::Thread() {
    startTimer(1000);

    QTimer *timer = new QTimer();

    connect(timer, &QTimer::timeout, [=] {
        qDebug() << "QTimer: " << timer->thread();
    });

    timer->start(1000);
}

void Thread::timerEvent(QTimerEvent *event) {
    qDebug() << this << QDateTime::currentDateTime();
}
