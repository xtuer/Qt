#ifndef MULTICASTRECIVER_H
#define MULTICASTRECIVER_H

#include <QObject>

class QUdpSocket;

class MulticastReciver : public QObject {
    Q_OBJECT
public:
    explicit MulticastReciver(QObject *parent = 0);
    ~MulticastReciver();

private slots:
    void readPendingDatagrams();

private:
    QUdpSocket *udpSocket;
};

#endif // MULTICASTRECIVER_H
