#include <QDebug>
#include <QVariant>
#include <QByteArray>
#include <QDataStream>

#include "User.h"

int main(int argc, char *argv[]) {
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    // [1] QDataStream 操作自定义类型的 QVariant 时需要先注册
    qRegisterMetaTypeStreamOperators<User>();

    // [2] 把对象转换为 QVariant
    QVariant aliVar = QVariant::fromValue(User(100, "Ali", "Secret"));
    QByteArray buffer; // 存储的载体

    // [3] 把 User 的 QVariant 写入 buffer
    QDataStream out(&buffer, QIODevice::WriteOnly);
    out << aliVar;

    // [4] 从 buffer 中读取对象
    QDataStream in(&buffer, QIODevice::ReadOnly);
    QVariant readedAliVar;
    in >> readedAliVar;
    User readedAli = readedAliVar.value<User>();

    // 输出: "ID: 100, Username: Ali, Password: Secret"
    qDebug() << QString("ID: %1, Username: %2, Password: %3")
                .arg(readedAli.id).arg(readedAli.username).arg(readedAli.password);

    return 0;
}
