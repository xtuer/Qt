#include "Message.h"
#include <QDataStream>

Message::Message(int type, const QString &content) : type(type), content(content) {}

QByteArray Message::toByteArray() const {
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << type << content; // 把成员变量写入 data
    return data;
}

Message Message::fromByteArray(const QByteArray &data) {
    Message msg;
    QDataStream in(data);
    in >> msg.type >> msg.content; // 从 data 中读取成员变量的数据
    return msg;
}

QString Message::toString() const {
    return QString("Type: %1, Content: %2").arg(type).arg(content);
}
