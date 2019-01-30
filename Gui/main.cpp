#include <QApplication>
#include <QDebug>
#include <QDateTime>

#include "Json.h"

/**
 * @brief 传感器数据，key 和 value 自定义，数组 data 为具体的数据
 */
struct SensorData {
/*
[
    {"WD334": "WENDU"},
    {"YL218": "YALI"},
    {"YL752": "YALI"},
    {"YL924": "YALI"}
]
*/
    QString key;   // 上面数据中的 key，不知道具体含义
    QString value; // 上面数据中的 value

    QList<qreal> data;

    SensorData(const QString &key, const QString &value, const QList<qreal> &data)
        : key(key), value(value), data(data) {
    }
};

/**
 * @brief 构建传感器数据的 JSON 字符串
 *
 * @param id         // ID，应该是发送的序号
 * @param deviceId   // 设备 ID
 * @param department // 部门
 * @param checkpoint // 检测点，例如沧浪医院第二消毒室
 * @param worker     // 操作员
 * @param times      // 数据的时间数组
 * @param data       // 传感器数据
 * @return 返回 JSON 字符串
 */
QString buildSensorJson(int id,
                        const QString &deviceId,
                        const QString &department,
                        const QString &checkpoint,
                        const QString &worker,
                        const QList<QDateTime> &times,
                        const QList<SensorData> &data) {
    Json json;

    json.set("id", id);
    json.set("jsonrpc", "2.0");
    json.set("method", "wenduyali_report");
    json.set("params.deviceid", deviceId);
    json.set("params.data.dept", department);
    json.set("params.data.checkpoint", checkpoint);
    json.set("params.data.op", worker);
    json.set("params.data.datetime", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));

    // 创建 params.data.define 和 params.data.data 数组
    QJsonArray defineArray;
    QJsonArray dataArray;

    QJsonObject defineTime;
    defineTime.insert("DATETIME", "DATETIME");
    defineArray << defineTime;

    QJsonArray timeArray;
    for (auto time : times) {
        timeArray << time.toString("yyyy-MM-dd HH:mm:ss");
    }
    dataArray << timeArray;

    for (auto d : data) {
        QJsonObject defineItem;
        defineItem.insert(d.key, d.value);
        defineArray.push_back(defineItem);

        QJsonArray dataItems;
        for (auto dd : d.data) {
            dataItems.push_back(dd);
        }
        dataArray.push_back(dataItems);
    }
    json.set("params.data.define", defineArray);
    json.set("params.data.data", dataArray);

    return json.toString();
}

int main(int argc, char *argv[]) {
    // 注意: 时间的个数和 SensorData 中数据 data 的元素个数相等
    QList<QDateTime> times;
    QDateTime time1 = QDateTime::currentDateTime();
    QDateTime time2 = QDateTime::currentDateTime();
    times << time1 << time2;

    SensorData data1("WD334", "WENDU", QList<qreal>() << 1 << 2 );
    SensorData data2("YL218", "YALI",  QList<qreal>() << 3 << 4 );
    QList<SensorData> datas;
    datas << data1 << data2;

    QString json = buildSensorJson(1, "xyz-1", "大海市沧浪区第二监督所", "沧浪医院第二消毒室", "胡一刀", times, datas);

    qDebug().noquote() << json;

    return 0;
}
