#include "BroadcastReceiver.h"

#include <QUdpSocket>
#include <QDebug>

BroadcastReceiver::BroadcastReceiver(QObject *parent) : QObject(parent) {
    udpSocket = new QUdpSocket();
    udpSocket->bind(13930, QUdpSocket::ShareAddress);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

BroadcastReceiver::~BroadcastReceiver() {
    delete udpSocket;
}

void BroadcastReceiver::readPendingDatagrams() {
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;

        // 不会存在粘包问题
        // pendingDatagramSize() returns the size of the first pending UDP datagram.
        // If there is no datagram available, this function returns -1.
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());

        qDebug() << QString::fromUtf8(datagram);
    }
}
