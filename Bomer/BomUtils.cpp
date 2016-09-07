#include "BomUtils.h"
#include "FileUtils.h"

#include <QString>
#include <QByteArray>
#include <QDebug>

bool BomUtils::hasBom(const QString &path) {
    QByteArray bom = FileUtils::readFile(path, 3);

    if (bom.size() != 3) { return false; }

    return (((quint8)bom[0] ^ 0xEF) == 0) &&
           (((quint8)bom[1] ^ 0xBB) == 0) &&
           (((quint8)bom[2] ^ 0xBF) == 0);
}

void BomUtils::insertBom(const QString &path) {
    // 1. 有 BOM 就不添加
    if (hasBom(path)) {
        qDebug() << QString("File '%1' already has BOM").arg(path);
        return;
    }

    // 2. 添加 BOM
    QByteArray content = FileUtils::readFile(path);
    content.insert(0, 0xBF);
    content.insert(0, 0xBB);
    content.insert(0, 0xEF);

    // 3. 覆盖原文件
    FileUtils::writeFile(path, content);
}

void BomUtils::removeBom(const QString &path) {
    // 1. 没有 BOM 就不删除
    if (!hasBom(path)) {
        qDebug() << QString("File '%1' has not BOM").arg(path);
        return;
    }

    // 2. 删除 BOM
    QByteArray content = FileUtils::readFile(path);
    content.remove(0, 3);

    // 3. 覆盖原文件
    FileUtils::writeFile(path, content);
}
