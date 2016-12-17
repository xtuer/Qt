#ifndef UNICASTRECEIVER_H
#define UNICASTRECEIVER_H

#include <QObject>

class QUdpSocket;

class UnicastReceiver : public QObject {
    Q_OBJECT
public:
    explicit UnicastReceiver(QObject *parent = 0);
    ~UnicastReceiver();

private slots:
    void readPendingDatagrams();

private:
    QUdpSocket *udpSocket;
};

#endif // UNICASTRECEIVER_H
