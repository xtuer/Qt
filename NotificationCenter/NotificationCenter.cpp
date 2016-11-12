#include "NotificationCenter.h"
#include <QList>

NotificationCenter::NotificationCenter() {}

NotificationCenter::~NotificationCenter() {}

// 添加观察者
void NotificationCenter::addObserver(int notificationId, QObject *observer) {
    if (NULL == observer) { return; }

    QSet<QObject*> &obs = observers[notificationId];
    obs.insert(observer);
}

// 删除观察者
void NotificationCenter::removeObserver(int notificationId, QObject *observer) {
    QSet<QObject*> &obs = observers[notificationId];
    obs.remove(observer);
}

// 删除观察者
void NotificationCenter::removeObserver(QObject *observer) {
    QList<int> notificationIds = observers.keys();

    foreach (int notificationId, notificationIds) {
        removeObserver(notificationId, observer);
    }
}

// 給观察者发送通知
void NotificationCenter::notify(int notificationId, const QByteArray &data) {
    QSet<QObject*> obs = observers[notificationId];

    foreach(QObject *observer, obs) {
        QMetaObject::invokeMethod(observer, "notified", Q_ARG(int, notificationId), Q_ARG(QByteArray, data)); // 使用 invokeMethod 是为可跨线程进行函数调用
    }
}
