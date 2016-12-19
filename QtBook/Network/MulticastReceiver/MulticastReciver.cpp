#include "MulticastReciver.h"

#include <QUdpSocket>
#include <QDebug>

MulticastReciver::MulticastReciver(QObject *parent) : QObject(parent) {
    udpSocket = new QUdpSocket();
    udpSocket->bind(QHostAddress::AnyIPv4, 13930, QUdpSocket::ShareAddress);
    udpSocket->joinMulticastGroup(QHostAddress("225.20.40.20"));

    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

MulticastReciver::~MulticastReciver() {
    delete udpSocket;
}

void MulticastReciver::readPendingDatagrams() {
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
