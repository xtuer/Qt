#include <QCoreApplication>
#include <QUdpSocket>
#include <QDebug>

QString generateMessage(int sn, char symbol);

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QHostAddress multicastAddress("225.20.40.20"); // Multicast 组的地址
    QUdpSocket *udpSocket = new QUdpSocket();
    int messageCount = 10;

    for (int i = 0; i < messageCount; ++i) {
        QString data = generateMessage(i, '*');
        // 使用 Unicast 发送消息
        udpSocket->writeDatagram(data.toUtf8(), multicastAddress, 13930);
        qDebug() << data;
    }

    return a.exec();
}

/**
 * @brief 创建一个字符串，格式为: "序号: + N个字符 + 结尾的x."
 * 例如："2:--------x."
 * @param sn - 代表字符串的顺序，便于区别
 * @return  - 返回随机生成的字符串
 */
QString generateMessage(int sn, char symbol) {
    QString str;
    str.fill(symbol, qrand() % 10 + 5);
    str.prepend(QString::number(sn) + ":");
    str.append("x.");

    return str;
}
