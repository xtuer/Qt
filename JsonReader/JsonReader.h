#ifndef JSONREADER_H
#define JSONREADER_H

#include <QJsonObject>

/**
 * Qt 的 Json 读取函数访问多层次的属性不够方便，这个类的目的就是能够使用带 "." 的路径格式访问 Json 的多级属性，例如
 * "id" 访问的是根节点下的 id，"user.address.street" 访问根节点下 user 的 address 的 street 的属性值。
 *
 * Json 例子：
 * {
 *     "id": 18191,
 *     "user": {
 *         "address": {
 *             "street": "Wiessenstrasse",
 *             "postCode": 100001
 *         },
 *         "childrenNames": ["Alice", "Bob", "John"]
 *     }
 * }
 *
 * 访问 id:     JsonReader.getInt("id")，返回 18191。
 * 访问 street: JsonReader.getString("user.address.street")，返回 "Wiessenstrasse"。
 * 访问 childrenNames: JsonReader.getStringList("user.childrenNames") 得到字符串列表("Alice", "Bob", "John")。
 *
 * 如果要找的属性不存在，则返回指定的默认值，如 "database.username.firstName" 表示的属性不存在，
 * 调用 JsonReader.getString("database.username.firstName", "defaultName")，由于要访问的属性不存在，
 * 得到的是一个空的 QJsonValue，所以返回我们指定的默认值 "defaultName"。
 *
 * 注意: Json 文件要使用 UTF-8 编码。
 */
class JsonReader {
public:
    /**
     * 使用 Json 字符串或者从文件读取 Json 内容创建 JsonReader 对象。
     * 如果 isFile 为 true，则 jsonOrJsonFilePath 为文件的路径
     * 如果 isFile 为 false，则 jsonOrJsonFilePath 为 Json 的字符串内容
     *
     * @param jsonOrJsonFilePath Json 的字符串内容或者 Json 文件的路径
     * @param isFile 为 true，则 jsonOrJsonFilePath 为文件的路径，为 false 则 jsonOrJsonFilePath 为 Json 的字符串内容
     */
    explicit JsonReader(const QString &jsonOrJsonFilePath, bool isFile = false);

    /**
     * 读取路径 path 对应属性的整数值
     * @param path 带 "." 的路径格
     * @param def 如果要找的属性不存在时返回的默认值
     * @param fromNode 从此节点开始查找，如果为默认值，则从 Json 的根节点开始查找
     * @return 整数值
     */
    int    getInt(const QString &path, int def = 0, const QJsonObject &fromNode = QJsonObject()) const;
    bool   getBool(const QString &path, bool def = false, const QJsonObject &fromNode = QJsonObject()) const;
    double getDouble(const QString &path, double def = 0.0, const QJsonObject &fromNode = QJsonObject()) const;

    QString     getString(const QString &path, const QString &def = QString(), const QJsonObject &fromNode = QJsonObject()) const;
    QStringList getStringList(const QString &path, const QJsonObject &fromNode = QJsonObject()) const;
    QJsonArray  getJsonArray(const QString &path, const QJsonObject &fromNode = QJsonObject()) const;
    QJsonValue  getJsonValue(const QString &path, const QJsonObject &fromNode = QJsonObject()) const;
    QJsonObject getJsonObject(const QString &path, const QJsonObject &fromNode = QJsonObject()) const;

private:
    QJsonObject root; // Json 的根节点
};

#endif // JSONREADER_H
