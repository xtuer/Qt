#include "JsonReader.h"
#include <QFile>
#include <QDebug>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QRegularExpression>
#include <QJsonParseError>

JsonReader::JsonReader(const QByteArray &json) {
    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(json, &error);
    rootObject = jsonDocument.object();

    if (QJsonParseError::NoError != error.error) {
        qDebug() << error.errorString() << ", Offset: " << error.offset;
    }
}

JsonReader::JsonReader(const QString &jsonFilePath) {
    QFile file(jsonFilePath); // Load Json file

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QJsonDocument jsonDocument = QJsonDocument::fromJson(file.readAll());
        rootObject = jsonDocument.object();
    } else {
        qDebug() << QString("Cannot open the file: %1").arg(jsonFilePath);
    }
}

int JsonReader::getInt(const QString &path, int def) const {
    return getJsonValue(path).toInt(def);
}

bool JsonReader::getBool(const QString &path, bool def) const {
    return getJsonValue(path).toBool(def);
}

QString JsonReader::getString(const QString &path, const QString &def) const {
    return getJsonValue(path).toString(def).trimmed();
}

QStringList JsonReader::getStringList(const QString &path) const {
    QStringList result;
    QJsonArray array = getJsonValue(path).toArray();

    for (QJsonArray::const_iterator iter = array.begin(); iter != array.end(); ++iter) {
        QJsonValue value = *iter;
        result << value.toString().trimmed();
    }

    return result;
}

QJsonValue JsonReader::getJsonValue(const QString &path) const {
    QStringList tokens = path.split(QRegularExpression("\\."));
    int size = tokens.size();
    QJsonObject object = rootObject;

    // 定位到要访问的属性的 parent，
    // 如 "user.address.street"，要访问的属性 "street" 的 parent 是 "address"
    for (int i = 0; i < size - 1; ++i) {
        if (object.isEmpty()) {
            return QJsonValue();
        }

        object = object.value(tokens.at(i)).toObject();
    }

    return object.value(tokens.last());
}
