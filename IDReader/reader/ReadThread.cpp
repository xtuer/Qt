#include "ReadThread.h"
#include "bean/Person.h"
#include "Constants.h"
#include "reader/CardReader.h"

#include <QDebug>

ReadThread::ReadThread(QObject *parent) : QThread(parent) {
    stopped = true;
    reader = new CardReader();
}

ReadThread::~ReadThread() {
    reader->disconnect();
    delete reader;
}

void ReadThread::stop() {
    stopped = true;
}

void ReadThread::run() {
    stopped = false;
    reader->connect();

    if (0 == reader->errorCode) {
        handleInfo(Constants::CODE_READ_READY, QString("连接身份证阅读器成功"));
    } else {
        handleInfo(reader->errorCode, reader->errorString);
    }

    while (reader->connected && !stopped) {
        Person p = reader->read();

        if (0 == reader->errorCode) {
            emit personReady(p);
        } else {
            handleInfo(reader->errorCode, reader->errorString);
        }

        QThread::msleep(1000);
    }

    stopped = true;
    reader->disconnect();
    handleInfo(reader->errorCode, reader->errorString);
}

void ReadThread::handleInfo(int code, const QString &message) {
    if (0 != code) {
        emit info(code, message);
    }
}
