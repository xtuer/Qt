#include "flatbuffers/flatbuffers.h"
#include "Person_generated.h"

#include <QString>
#include <QDebug>
#include <QFile>

void testRead();
void testWrite();
void writeToFile(const char *data, int size);
QByteArray readFromFile();

int main(int argc, char *argv[]) {
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    testRead();

    return 0;
}

void testWrite() {
    flatbuffers::FlatBufferBuilder builder;

    // [1] 创建 Person 对象
    QByteArray nameBA = QString("道格拉斯·狗").toUtf8();
    auto name = builder.CreateString(nameBA.constData()); // 不能直接用 QString("道格拉斯·狗").toUtf8().constData()
    auto root = com::xtuer::bean::CreatePerson(builder, name, 30);
    com::xtuer::bean::FinishPersonBuffer(builder, root);

    // [2] 序列化 Person 对象为字节码
    const char *buffer = reinterpret_cast<const char *>(builder.GetBufferPointer());

    // [3] 保存字节码到文件，也可以进行网络传输
    writeToFile(buffer, builder.GetSize());
}

void testRead() {
    // [1] 从文件读取字节码，也可以是使用 Socket 传输得到的
    QByteArray data = readFromFile();

    // [2] 把字节码反序列化为 Person 对象
    const com::xtuer::bean::Person *p = com::xtuer::bean::GetPerson(data.constData());

    qDebug() << QString("Name: %1, Age: %2").arg(QString(p->name()->c_str())).arg(p->age()); // OK: 默认使用 UTF-8 创建 QString
    qDebug() << QString::fromUtf8(p->name()->c_str());      // OK: 证明 FlatBuffer 使用的是 UTF-8 存储字符串
    qDebug() << QString::fromLocal8Bit(p->name()->c_str()); // OK: Mac 默认就是使用 UTF-8
    qDebug() << QString::fromLatin1(p->name()->c_str());    // Error: 因为不是使用 Latin1 存储字符串
}

void writeToFile(const char *data, int size) {
    QFile file("/Users/Biao/Desktop/person.data");
    if (!file.open(QIODevice::WriteOnly)) { return; }

    file.write(data, size);
}

QByteArray readFromFile() {
    QFile file("/Users/Biao/Desktop/person.data");
    if (!file.open(QIODevice::ReadOnly)) { return QByteArray(); }

    return file.readAll();
}
