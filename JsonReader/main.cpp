#include <QApplication>
#include <QDebug>
#include <QJsonArray>
#include <QDebug>
#include "JsonReader.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 从文件里读取 Json
    JsonReader reader("/Users/Biao/Desktop/x.json", true);

    // 第一级
    qDebug() << reader.getString("message");
    qDebug() << reader.getString("non-exit", "不存在");

    // 第二级
    qDebug() << reader.getString("admin.username");
    qDebug() << reader.getString("admin.password");

    // 第二级的数组
    qDebug() << reader.getStringList("admin.roles");

    // 第二级的数组中的值
    QJsonArray array = reader.getJsonArray("roomEnrollmentList");
    QJsonObject fromNode = array.at(0).toObject();
    qDebug() << reader.getString("examineeName", "", fromNode); // 传入开始查找的节点

    // 从字符串里读取 Json
    JsonReader reader2("{\"data\": {\"userId\": 12345}}");
    qDebug() << reader2.getInt("data.userId");

    return a.exec();
}
