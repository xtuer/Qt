#include "Thread.h"
#include <QDateTime>
#include <QDebug>

Thread::Thread() {
    qDebug() << "2-Thread::Thread(): " << QThread::currentThread(); // [+] 属于 Ui 线程
}

void Thread::run() {
    // [-] 属于 Thread 的线程
    // 虽然线程对象 thread 属于 Ui 线程，但是它的 run 函数属于 Thread 自己的线程
    qDebug() << "4-Thread.run(): " << QThread::currentThread();

    while (true) {
        emit beat();
        QThread::msleep(2000);
    }
}
