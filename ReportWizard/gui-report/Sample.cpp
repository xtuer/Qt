#include "Sample.h"
#include "ui_Sample.h"
#include "bean/ReportSettings.h"

Sample::Sample(QWidget *parent) : QWidget(parent), ui(new Ui::Sample) {
    ui->setupUi(this);

    // [3] 构造函数中调用 ReportWizardBase::initialize(ui->stackedWidget)
    ReportWizardBase::initialize(ui->stackedWidget);
}

Sample::~Sample() {
    delete ui;
}

// [4.1] 报告设置显示到界面上, 即把设置显示到对应的 widget 上
void Sample::setReportSettings(ReportSettings settings) {
    ui->lineEdit->setText(settings.address);
}

// [4.2] 获取报告设置, 即把 widget 上的设置提取出来创建返回 ReportSettings 对象
ReportSettings Sample::getReportSettings() {
    ReportSettings settings;
    settings.address = ui->lineEdit->text().trimmed();

    return settings;
}
