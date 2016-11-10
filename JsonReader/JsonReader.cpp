#include "JsonReader.h"
#include <QFile>
#include <QDebug>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QRegularExpression>
#include <QJsonParseError>

JsonReader::JsonReader(const QString &jsonOrJsonFilePath, bool isFile) {
    QByteArray json; // json 的内容

    // 如果传人的是 Json 文件的路径，则读取内容
    if (isFile) {
        QFile file(jsonOrJsonFilePath);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            json = file.readAll();
        } else {
            qDebug() << QString("Cannot open the file: %1").arg(jsonOrJsonFilePath);
        }
    } else {
        json = jsonOrJsonFilePath.toUtf8();
    }

    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(json, &error);
    root = jsonDocument.object();

    if (QJsonParseError::NoError != error.error) {
        qDebug() << error.errorString() << ", Offset: " << error.offset;
    }
}

int JsonReader::getInt(const QString &path, int def, const QJsonObject &fromNode) const {
    return getJsonValue(path, fromNode.isEmpty() ? root : fromNode).toInt(def);
}

bool JsonReader::getBool(const QString &path, bool def, const QJsonObject &fromNode) const {
    return getJsonValue(path, fromNode.isEmpty() ? root : fromNode).toBool(def);
}

double JsonReader::getDouble(const QString &path, double def, const QJsonObject &fromNode) const {
    return getJsonValue(path, fromNode.isEmpty() ? root : fromNode).toDouble(def);
}

QString JsonReader::getString(const QString &path, const QString &def, const QJsonObject &fromNode) const {
    return getJsonValue(path, fromNode.isEmpty() ? root : fromNode).toString(def);
}

QStringList JsonReader::getStringList(const QString &path, const QJsonObject &fromNode) const {
    QStringList result;
    QJsonArray array = getJsonValue(path, fromNode.isEmpty() ? root : fromNode).toArray();

    for (QJsonArray::const_iterator iter = array.begin(); iter != array.end(); ++iter) {
        QJsonValue value = *iter;
        result << value.toString();
    }

    return result;
}

QJsonArray JsonReader::getJsonArray(const QString &path, const QJsonObject &fromNode) const {
    return getJsonValue(path, fromNode.isEmpty() ? root : fromNode).toArray();
}

QJsonObject JsonReader::getJsonObject(const QString &path, const QJsonObject &fromNode) const {
    return getJsonValue(path, fromNode.isEmpty() ? root : fromNode).toObject();
}

QJsonValue JsonReader::getJsonValue(const QString &path, const QJsonObject &fromNode) const {
    QJsonObject parent(fromNode.isEmpty() ? root : fromNode);

    QStringList tokens = path.split(QRegularExpression("\\."));
    int size = tokens.size();

    // 定位到要访问的属性的 parent，
    // 如 "user.address.street"，要访问的属性 "street" 的 parent 是 "address"
    for (int i = 0; i < size - 1; ++i) {
        if (parent.isEmpty()) {
            return QJsonValue();
        }

        parent = parent.value(tokens.at(i)).toObject();
    }

    return parent.value(tokens.last());
}
