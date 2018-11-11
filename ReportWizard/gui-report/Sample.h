#ifndef SAMPLE_H
#define SAMPLE_H

#include <QWidget>
#include "ReportWizardBase.h"

namespace Ui {
class Sample;
}

/**
 * @brief 报告设置的模板, 使用步骤如下:
 *
 * 1. 继承 ReportWizardBase
 * 2. Ui 中使用 QStackedWidget 创建多个设置的页面
 * 3. 构造函数中调用 ReportWizardBase::initialize(ui->stackedWidget) 初始化 ReportWizardBase
 * 4. 实现报告设置的纯虚函数
 *    setReportSettings():  报告设置显示到界面上, 即把设置显示到对应的 widget 上
 *    getResportSettings(): 获取报告设置, 即把 widget 上的设置提取出来创建返回 ReportSettings 对象
 */
class Sample : public QWidget, public ReportWizardBase { // [1] 继承 ReportWizardBase
    Q_OBJECT

public:
    explicit Sample(QWidget *parent = nullptr);
    ~Sample() override;

    // [4.1] 报告设置显示到界面上, 即把设置显示到对应的 widget 上
    void setReportSettings(ReportSettings settings) override;

    // [4.2] 获取报告设置, 即把 widget 上的设置提取出来创建返回 ReportSettings 对象
    ReportSettings getReportSettings() override;

private:
    Ui::Sample *ui;
};

#endif // SAMPLE_H
