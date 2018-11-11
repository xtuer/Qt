#ifndef REPORTWIZARDBASE_H
#define REPORTWIZARDBASE_H

class ReportSettings;
class QStackedWidget;

class ReportWizardBase {
public:
    ReportWizardBase();
    virtual ~ReportWizardBase();

    virtual void setReportSettings(ReportSettings settings) = 0; // 报告设置显示到界面上, 即把设置显示到对应的 widget 上
    virtual ReportSettings getReportSettings() = 0;              // 获取报告设置, 即把 widget 上的设置提取出来创建返回 ReportSettings 对象

    bool prevPage(); // 跳转到 wizard 的上一页, 如果是第一页返回 false (不能再往前跳转了), 否则返回 true
    bool nextPage(); // 跳转到 wizard 的下一页, 如果是末一页返回 false (不能再往后跳转了), 否则返回 true

protected:
    void initialize(QStackedWidget *stackedWidget); // 初始化

private:
    QStackedWidget *stackedWidget = nullptr;
};

#endif // REPORTWIZARDBASE_H
