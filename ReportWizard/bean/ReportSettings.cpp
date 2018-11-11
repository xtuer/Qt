#include "ReportSettings.h"
#include "util/Json.h"

// 创建空的 ReportSettings 对象
ReportSettings::ReportSettings() {

}

// 用 JSON 格式表示的 ReportSettings 创建 ReportSettings 对象
ReportSettings ReportSettings::fromJson(const QString &jsonString) {
    Json json(jsonString);
    ReportSettings settings;

    // 设备类型
    settings.deviceType     = json.getInt("deviceType");
    settings.deviceTypeName = json.getString("deviceTypeName");

    settings.timeZoneChecked = json.getBool("timeZoneChecked");
    settings.killTimeChecked = json.getBool("killTimeChecked");
    settings.averageChecked  = json.getBool("averageChecked");
    settings.maxChecked      = json.getBool("maxChecked");
    settings.minChecked      = json.getBool("minChecked");
    settings.bodongChecked   = json.getBool("bodongChecked");
    settings.valueF0Checked  = json.getBool("valueF0Checked");
    settings.valueA0Checked  = json.getBool("valueA0Checked");

    // 通用信息
    settings.companyName    = json.getString("companyName");
    settings.department     = json.getString("department");
    settings.address        = json.getString("address");
    settings.phoneNumber    = json.getString("phoneNumber");
    settings.expert         = json.getString("expert");
    settings.deviceName     = json.getString("deviceName");
    settings.deviceModel    = json.getString("deviceModel");
    settings.deviceSerial   = json.getString("deviceSerial");
    settings.deviceProvider = json.getString("deviceProvider");

    // 灭菌参数
    settings.killTemp      = json.getDouble("killTemp");
    settings.killTime      = json.getInt("killTime");
    settings.curveProperty = json.getInt("curveProperty");
    settings.curvePress    = json.getInt("curvePress");
    settings.buDian        = json.getInt("buDian");

    // 报告参数
    settings.report1 = json.getBool("report1");
    settings.report2 = json.getBool("report2");
    settings.report3 = json.getBool("report3");
    settings.report4 = json.getBool("report4");

    return settings;
}

// 把 ReportSettings 对象转换为 JSON 字符串的表示，方便存储到数据库、文件等
QString ReportSettings::toJson() const {
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
