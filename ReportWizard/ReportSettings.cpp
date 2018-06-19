#include "ReportSettings.h"
#include "Json.h"

// 创建空的 ReportSettings 对象
ReportSettings::ReportSettings() {

}

// 用 JSON 格式表示的 ReportSettings 创建 ReportSettings 对象
ReportSettings::ReportSettings(const QString &jsonString) {
    Json json(jsonString);

    // 设备类型
    deviceType     = json.getInt("deviceType");
    deviceTypeName = json.getString("deviceTypeName");

    timeZoneChecked = json.getBool("timeZoneChecked");
    killTimeChecked = json.getBool("killTimeChecked");
    averageChecked  = json.getBool("averageChecked");
    maxChecked      = json.getBool("maxChecked");
    minChecked      = json.getBool("minChecked");
    bodongChecked   = json.getBool("bodongChecked");
    valueF0Checked  = json.getBool("valueF0Checked");
    valueA0Checked  = json.getBool("valueA0Checked");

    // 通用信息
    companyName    = json.getString("companyName");
    department     = json.getString("department");
    address        = json.getString("address");
    phoneNumber    = json.getString("phoneNumber");
    expert         = json.getString("expert");
    deviceName     = json.getString("deviceName");
    deviceModel    = json.getString("deviceModel");
    deviceSerial   = json.getString("deviceSerial");
    deviceProvider = json.getString("deviceProvider");

    // 灭菌参数
    killTemp      = json.getDouble("killTemp");
    killTime      = json.getInt("killTime");
    curveProperty = json.getInt("curveProperty");
    curvePress    = json.getInt("curvePress");
    buDian        = json.getInt("buDian");

    // 报告参数
    report1 = json.getBool("report1");
    report2 = json.getBool("report2");
    report3 = json.getBool("report3");
    report4 = json.getBool("report4");
}

// 把 ReportSettings 对象转换为 JSON 字符串的表示，方便存储到数据库、文件等
QString ReportSettings::toJson() {
    Json json;

    json.set("deviceType", deviceType);
    json.set("deviceTypeName", deviceTypeName);

    json.set("timeZoneChecked", timeZoneChecked);
    json.set("killTimeChecked", killTimeChecked);
    json.set("averageChecked", averageChecked);
    json.set("maxChecked", maxChecked);
    json.set("minChecked", minChecked);
    json.set("bodongChecked", bodongChecked);
    json.set("valueF0Checked", valueF0Checked);
    json.set("valueA0Checked", valueA0Checked);

    json.set("companyName", companyName);
    json.set("department", department);
    json.set("address", address);
    json.set("phoneNumber", phoneNumber);
    json.set("expert", expert);
    json.set("deviceName", deviceName);
    json.set("deviceModel", deviceModel);
    json.set("deviceSerial", deviceSerial);
    json.set("deviceProvider", deviceProvider);

    json.set("killTemp", killTemp);
    json.set("killTime", killTime);
    json.set("curveProperty", curveProperty);
    json.set("curvePress", curvePress);
    json.set("buDian", buDian);

    json.set("report1", report1);
    json.set("report2", report2);
    json.set("report3", report3);
    json.set("report4", report4);

    return json.toString();
}
