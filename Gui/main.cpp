#include <QDebug>
#include <QApplication>
#include <QPushButton>
#include <QThread>
#include "Thread.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Thread thread;
    thread.start();

    qDebug() << "Main: " << QThread::currentThread();

    return app.exec();
}
