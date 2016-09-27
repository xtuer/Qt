#ifndef JSONREADER_H
#define JSONREADER_H

#include <QJsonObject>

/**
使用带 "." 的路径格式访问属性，例如 "id" 访问的是根节点下的 id，
"user.address.street" 访问根节点下 user 的 address 的 street。

Json 例子：
{
    "id": 18191,
    "user": {
        "address": {
            "street": "Wiessenstrasse",
            "postCode": 100001
        }
    }
}

访问 id:     JsonReader.getInt("id")，返回 18191。
访问 street: JsonReader.getString("user.address.street")，返回 "Wiessenstrasse"。

如果要找的属性不存在，则返回空的 QJsonValue，如 "database.username.firstName" 表示的属性就不存在，
调用 JsonReader.getString("database.username.firstName", "defaultName")，由于要访问的属性不存在，
得到的是一个空的 QJsonValue，转化为 QString，返回我们指定的默认值 "defaultName"。

返回的字符串使用 trimmed() 去掉了两头的空白字符。
Json 文件使用 UTF-8 编码。
*/
class JsonReader {
public:
    explicit JsonReader(const QByteArray &json);
    explicit JsonReader(const QString &jsonFilePath);

    int  getInt(const QString &path, int def = 0) const;
    bool getBool(const QString &path, bool def = false) const;
    QString     getString(const QString &path, const QString &def = QString()) const;
    QStringList getStringList(const QString &path) const;
    QJsonValue  getJsonValue(const QString &path) const;

//private:
    QJsonObject rootObject;
};

#endif // JSONREADER_H
