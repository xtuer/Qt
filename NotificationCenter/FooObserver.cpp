#include "FooObserver.h"
#include <QDebug>
#include <QThread>
#include "Message.h"

FooObserver::FooObserver(QObject *parent) : QObject(parent) {}

void FooObserver::notified(int notificationId, const QByteArray &data) {
    if (3 == notificationId) {
        // notificationId 为 3 说明接收到的通知是 Message
        qDebug() << "NotificationID: " << notificationId << " - " << "FooObserver: " << Message::fromByteArray(data).toString();
    } else if (4 == notificationId) {
        qDebug() << "NotificationID: " << notificationId << " - " << "FooObserver: " << data << "- in thread: " << QThread::currentThread();
    } else {
        qDebug() << "NotificationID: " << notificationId << " - " << "FooObserver: " << data;
    }
}
