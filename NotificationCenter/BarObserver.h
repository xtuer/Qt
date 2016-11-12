#ifndef BAROBSERVER_H
#define BAROBSERVER_H

#include <QObject>

class BarObserver : public QObject {
    Q_OBJECT
public:
    explicit BarObserver(QObject *parent = 0);
    Q_INVOKABLE void notified(int notificationId, const QByteArray &data);
};

#endif // BAROBSERVER_H
