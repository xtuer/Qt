#ifndef MESSAGE_H
#define MESSAGE_H

#include <QDebug>
#include <QString>
#include <QByteArray>

class Message {
public:
    Message(int type = 0, const QString &content = "");

    QByteArray toByteArray() const; // 序列化 Message 为 QByteArray
    static Message fromByteArray(const QByteArray &data); // 从 QByteArray 反序列化为 Message

    QString toString() const; // 把 Message 格式化为字符串，方便调试

private:
    int type;
    QString content;
};

#endif // MESSAGE_H
