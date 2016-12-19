#ifndef BROADCASTRECEIVER_H
#define BROADCASTRECEIVER_H
#include <QObject>

class QUdpSocket;

class BroadcastReceiver : public QObject {
    Q_OBJECT
public:
    explicit BroadcastReceiver(QObject *parent = 0);
    ~BroadcastReceiver();

private slots:
    void readPendingDatagrams();

private:
    QUdpSocket *udpSocket;
};

#endif // BROADCASTRECEIVER_H
