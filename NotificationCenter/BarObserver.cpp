#include "BarObserver.h"
#include <QDebug>

BarObserver::BarObserver(QObject *parent) : QObject(parent) {}

void BarObserver::notified(int notificationId, const QByteArray &data) {
    qDebug() << "NotificationID: " << notificationId << " - " << "BarObserver: " << data;
}
