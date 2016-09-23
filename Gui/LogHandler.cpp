#include "LogHandler.h"
#include <QDebug>
#include <QDateTime>
#include <QMutexLocker>

QDate  LogHandler::today;
QTimer LogHandler::timer;
QMutex LogHandler::mutex;
QFile* LogHandler::file = NULL;

void LogHandler::initialize() {
    today = QDate::currentDate();

    file = new QFile("log.txt");
    file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);

    timer.setInterval(1000);
    timer.start();

    QObject::connect(&timer, &QTimer::timeout, [] {
        QMutexLocker locker(&mutex);

        // 重命名文件
        file->close();
        file->rename(QDateTime::currentDateTime().toString("hh-mm-ss.log"));
        delete file;

        file = new QFile("log.txt");
        file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    });
}

void LogHandler::destroy() {
    file->close();
    delete file;
}
