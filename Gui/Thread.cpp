#include "Thread.h"
#include <QThreadStorage>
#include <QDebug>
#include <QRandomGenerator>
#include <QCoreApplication>

Thread::Thread() {

}

void foo() {
    static QThreadStorage<QString> storage;

    if (storage.hasLocalData()) {
        qDebug() << "Fetch thread data: " << storage.localData();
    } else {
        QString id = "my_db_" + QString::number(quint64(QThread::currentThread()), 16);
        storage.setLocalData(id);
        qDebug() << "Create thread data: " << id;

        QObject::connect(QThread::currentThread(), &QThread::finished, qApp, [id] {
            qDebug() << "finished: " << id;
        });
    }
}

void Thread::run() {
    for (int i = 0; i < 5; ++i) {
        foo();
        QThread::sleep(ulong(QRandomGenerator::global()->bounded(3))); // [0, 100)
    }
}
