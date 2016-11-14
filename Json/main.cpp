#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include "Json.h"

int main(int argc, char *argv[]) {
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    // 使用字符串创建 Json
    Json j("{\"message\": \"Welcome\"}");
    qDebug() << j.getString("message");

    // 从文件读取 Json
    Json json("/Users/Biao/Documents/workspace/Qt/Json/x.json", true);

    // 第一级
    qDebug() << json.getString("message");
    qDebug() << json.getString("non-exit", "不存在");

    // 第二级
    qDebug() << json.getString("admin.username");
    qDebug() << json.getString("admin.password");

    // 第二级的数组
    qDebug() << json.getStringList("admin.roles");

    // 第二级的数组中的值
    QJsonArray array = json.getJsonArray("roomEnrollmentList");
    QJsonObject fromNode = array.at(0).toObject();
    qDebug() << json.getString("examineeName", "", fromNode); // 传入开始查找的节点

    // 修改 Json，创建 foo.bar.avatar
    json.set("foo.bar.avatar", "Sparta");
    qDebug() << json.getJsonValue("foo.bar");

    // 修改 Json，給 foo.bar 创建 fruit，和給 foo 创建 names 数组
    QJsonObject bar = json.getJsonObject("foo.bar");
    bar.insert("fruit", "Apple");
    json.set("foo.bar", bar);
    json.set("foo.names", QStringList() << "One" << "Two" << "Three");
    qDebug() << json.toString(QJsonDocument::Compact);

    // 保存到文件
    json.save("/Users/Biao/Desktop/xr.json");

    return 0;
}
