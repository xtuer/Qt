#include "ReadingThread.h"
#include <QFile>
#include <QTextStream>
#include <QTextEdit>
#include <QMetaObject>

ReadingThread::ReadingThread(QTextEdit *textEdit, QObject *parent) : QThread(parent), textEdit(textEdit) {
}

void ReadingThread::stop() {
    stopped = true;
}

void ReadingThread::run() {
    stopped = false;  // 线程开始执行时设置 stopped 为 false
    QFile file("/Users/Biao/Qt5.9.2/Docs/Qt-5.9.2/qtgui/qtgui.index");

    if (!file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        return;
    }

    QTextStream in(&file);

    // 当 stopped 为 true，或者 atEnd() 为 true 时结束 while 循环
    while (!stopped && !in.atEnd()) {
        QString line = in.readLine();
        QMetaObject::invokeMethod(textEdit, "append", Q_ARG(QString, line));
        QThread::msleep(1);
    }
}
