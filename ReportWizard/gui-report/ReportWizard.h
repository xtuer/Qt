#ifndef REPORTWIZARD_H
#define REPORTWIZARD_H

#include <QWidget>
#include <QHash>

class ReportSettings;
class ReportWizardBase;

namespace Ui {
class ReportWizard;
}

/**
 * 设备的报告配置向导
 * 逻辑：
 *   1. 切换设备类型的时候会到数据库读取此类型对应的配置显示都界面上
 *   2. 点击确定按钮时会把当前设备类型的配置保存到数据库里，并发射信号 reportSettingsChanged
 *   3. 显示报表向导的时候从数据库里读取下标为 0 的设备的配置，即环境试验设备的
 */
class ReportWizard : public QWidget {
    Q_OBJECT

public:
    explicit ReportWizard(QWidget *parent = nullptr);
    ~ReportWizard();

    ReportSettings loadReportSettings(int deviceType);       // 从数据库中加载设备的报告设置
    void saveReportSettings(const ReportSettings &settings); // 保存设备的报告设置到数据库

signals:
    // 点击确定按钮时会把当前设备类型的配置保存到数据库里，并发射信号 reportSettingsChanged
    void reportSettingsChanged(const ReportSettings &settings);

private:
    void initialize();   // 初始化
    void handleEvents(); // 信号槽处理
    ReportWizardBase* getCurrentSettingsWizard() const; // 获取当前设备配置的 Widget

    Ui::ReportWizard *ui;
};

#endif // REPORTWIZARD_H
