#include "FileUtils.h"
#include <QFile>
#include <QString>
#include <QByteArray>
#include <QDebug>

QByteArray FileUtils::readFile(const QString &path, int maxSize) {
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << QString("Open file '%1' failed").arg(path);
        return QByteArray();
    }

    QByteArray content;

    if (maxSize !=0) {
        content = file.read(maxSize);
    } else {
        content = file.readAll();
    }

    file.close();

    return content;
}

void FileUtils::writeFile(const QString &path, const QByteArray &content) {
    QFile file(path);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << QString("Open file '%1' failed").arg(path);
        return;
    }

    file.write(content);
    file.flush();
    file.close();
}
