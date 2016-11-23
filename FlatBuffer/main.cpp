#include "flatbuffers/flatbuffers.h"
#include "Person_generated.h"

#include <QString>
#include <QDebug>
#include <QFile>

int main(int argc, char *argv[]) {
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    QFile file("/Users/Biao/Desktop/person.data");
    if (!file.open(QIODevice::ReadOnly)) { return -10086; }
    QByteArray buffer = file.readAll();

    const com::xtuer::bean::Person *p = com::xtuer::bean::GetPerson(buffer.constData());
    qDebug() << QString("Name: %1, Age: %2").arg(QString(p->name()->c_str())).arg(p->age());

    qDebug() << QString::fromUtf8(p->name()->c_str());      // OK: 证明 FlatBuffer 使用的是 UTF-8 存储字符串
    qDebug() << QString::fromLocal8Bit(p->name()->c_str()); // OK: Mac 默认就是使用 UTF-8
    qDebug() << QString::fromLatin1(p->name()->c_str());    // Error: 因为不是使用 Latin1 存储字符串

    return 0;
}
