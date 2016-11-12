#ifndef FOOOBSERVER_H
#define FOOOBSERVER_H

#include <QObject>

class FooObserver : public QObject {
    Q_OBJECT
public:
    explicit FooObserver(QObject *parent = 0);
    Q_INVOKABLE void notified(int notificationId, const QByteArray &data);
};

#endif // FOOOBSERVER_H
