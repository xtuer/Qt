#ifndef REPORTSETTINGS_H
#define REPORTSETTINGS_H
#include <QString>

class ReportSettings {
public:
    /**
     * 创建默认的 ReportSettings 对象
     */
    ReportSettings();

    /**
     * 用 JSON 格式表示的 ReportSettings 创建 ReportSettings 对象
     *
     * @param json ReportSettings 的 JSON 字符串表示
     */
    static ReportSettings fromJson(const QString &json);

    /**
     * 把 ReportSettings 对象转换为 JSON 字符串的表示，方便存储到数据库、文件等
     *
     * @return JSON 字符串
     */
    QString toJson() const;

    // 设备类型
    // 0: 环境试验设备
    // 1: 蒸汽灭菌设备
    // 2: 干热灭菌设备
    // 3: 清洗消毒设备
    // 4: 微波消解设备
    int deviceType = 0;
    QString deviceTypeName; // 设备类型名称，为了方便查看，和 deviceType 对应

    bool timeZoneChecked = false; // 是否选中灭菌时段
    bool killTimeChecked = false; // 是否选中测量时间
    bool averageChecked  = false; // 是否选中测量均值
    bool maxChecked      = false; // 是否选中最大值
    bool minChecked      = false; // 是否选中最小值
    bool bodongChecked   = false; // 是否选中波动度
    bool valueF0Checked  = false; // 是否选中测量 F0 值
    bool valueA0Checked  = false; // 是否选中测量 A0 值

    // 通用信息
    QString companyName;    // 公司名称
    QString department;     // 部门名称
    QString address;        // 详细地址
    QString phoneNumber;    // 电话号码
    QString expert;         // 作业专家
    QString deviceName;     // 设备名称
    QString deviceModel;    // 设备型号
    QString deviceSerial;   // 设备编号
    QString deviceProvider; // 设备厂家

    // 灭菌参数
    double killTemp = 0; // 灭菌温度
    int    killTime = 0; // 灭菌时间
    int    curveProperty = 0; // 毛刺处理：0 为曲线平滑禁止，1 为曲线平滑使能
    int    curvePress    = 0; // 压力辅助：0 为饱和蒸汽辅助禁用，1 为饱和蒸汽辅助使能
    int    buDian        = 0; // 布点开关：0 为分析报告附加点图禁止，1 为分析报告附加点图使能

    // 报告参数
    bool report1 = false; // 是否选中统计表格报告
    bool report2 = false; // 是否选中温度 F0 值报告
    bool report3 = false; // 是否选中综合分析报告
    bool report4 = false; // 是否选中审计追踪报告
};

#endif // REPORTSETTINGS_H
