#include "CardReaderThread.h"
#include "bean/Person.h"
#include "Constants.h"
#include "reader/CardReader.h"

#include <QDebug>

CardReaderThread::CardReaderThread(QObject *parent) : QThread(parent) {
    stopped = true;
    reader = new CardReader();
}

CardReaderThread::~CardReaderThread() {
    reader->disconnect();
    delete reader;
}

void CardReaderThread::stop() {
    stopped = true;
}

void CardReaderThread::run() {
    stopped = false;
    reader->connect();

    qDebug() << "==> " << reader->errorCode << reader->errorString;

    if (0 == reader->errorCode) {
        // 连接身份证刷卡器成功
        handleInfo(Constants::CODE_READ_READY, Constants::INFO_READ_READY);
    } else {
        // 连接身份证刷卡器失败
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

void CardReaderThread::handleInfo(int code, const QString &message) {
    if (0 != code) {
        emit info(code, message);
    }
}
