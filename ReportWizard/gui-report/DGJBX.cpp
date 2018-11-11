#include "DGJBX.h"
#include "ui_DGJBX.h"

#include "bean/ReportSettings.h"

DGJBX::DGJBX(QWidget *parent) : QWidget(parent), ui(new Ui::DGJBX) {
    ui->setupUi(this);
    ReportWizardBase::initialize(ui->stackedWidget);
}

DGJBX::~DGJBX() {
    delete ui;
}

void DGJBX::setReportSettings(ReportSettings settings) {
    // 设备类型
    ui->timeZoneCheckBox->setChecked(settings.timeZoneChecked);
    ui->killTimeCheckBox->setChecked(settings.killTimeChecked);
    ui->averageCheckBox->setChecked(settings.averageChecked);
    ui->maxCheckBox->setChecked(settings.maxChecked);
    ui->minCheckBox->setChecked(settings.minChecked);
    ui->bodongCheckBox->setChecked(settings.bodongChecked);
    ui->valueF0CheckBox->setChecked(settings.valueF0Checked);
    ui->valueA0CheckBox->setChecked(settings.valueA0Checked);

    // 通用信息
    ui->companyNameLineEdit->setText(settings.companyName);
    ui->departmentLineEdit->setText(settings.department);
    ui->addressLineEdit->setText(settings.address);
    ui->phoneNumberLineEdit->setText(settings.phoneNumber);
    ui->expertLineEdit->setText(settings.expert);
    ui->deviceNameLineEdit->setText(settings.deviceName);
    ui->deviceModelLineEdit->setText(settings.deviceModel);
    ui->deviceSerialLineEdit->setText(settings.deviceSerial);
    ui->deviceProviderLineEdit->setText(settings.deviceProvider);

    // 灭菌参数
    ui->killTempLineEdit->setText(QString::number(settings.killTemp));
    ui->killTimeLineEdit->setText(QString::number(settings.killTime));
    ui->curvePropertyComboBox->setCurrentIndex(settings.curveProperty);;
    ui->curvePressComboBox->setCurrentIndex(settings.curvePress);
    ui->buDianComboBox->setCurrentIndex(settings.buDian);

    // 报告参数
    ui->report1CheckBox->setChecked(settings.report1);
    ui->report2CheckBox->setChecked(settings.report2);
    ui->report3CheckBox->setChecked(settings.report3);
    ui->report4CheckBox->setChecked(settings.report4);
}

ReportSettings DGJBX::getReportSettings() {
    ReportSettings settings;

    // 设备类型
    settings.deviceType      = 0; // 设备类型
    settings.deviceTypeName  = "冻干机 / 冰箱";  // 设备类型名称

    settings.timeZoneChecked = ui->timeZoneCheckBox->isChecked(); // 是否选中灭菌时段
    settings.killTimeChecked = ui->killTimeCheckBox->isChecked(); // 是否选中测量时间
    settings.averageChecked  = ui->averageCheckBox->isChecked();  // 是否选中测量均值
    settings.maxChecked      = ui->maxCheckBox->isChecked();      // 是否选中最大值
    settings.minChecked      = ui->minCheckBox->isChecked();      // 是否选中最小值
    settings.bodongChecked   = ui->bodongCheckBox->isChecked();   // 是否选中波动度
    settings.valueF0Checked  = ui->valueF0CheckBox->isChecked();  // 是否选中测量 F0 值
    settings.valueA0Checked  = ui->valueA0CheckBox->isChecked();  // 是否选中测量 A0 值

    // 通用信息
    settings.companyName    = ui->companyNameLineEdit->text().trimmed();    // 公司名称
    settings.department     = ui->departmentLineEdit->text().trimmed();     // 部门名称
    settings.address        = ui->addressLineEdit->text().trimmed();        // 详细地址
    settings.phoneNumber    = ui->phoneNumberLineEdit->text().trimmed();    // 电话号码
    settings.expert         = ui->expertLineEdit->text().trimmed();         // 作业专家
    settings.deviceName     = ui->deviceNameLineEdit->text().trimmed();     // 设备名称
    settings.deviceModel    = ui->deviceModelLineEdit->text().trimmed();    // 设备型号
    settings.deviceSerial   = ui->deviceSerialLineEdit->text().trimmed();   // 设备编号
    settings.deviceProvider = ui->deviceProviderLineEdit->text().trimmed(); // 设备厂家

    // 灭菌参数
    settings.killTemp = ui->killTempLineEdit->text().trimmed().toDouble(); // 灭菌温度
    settings.killTime = ui->killTimeLineEdit->text().trimmed().toInt();    // 灭菌时间
    settings.curveProperty = ui->curvePropertyComboBox->currentIndex();    // 毛刺处理：0 为曲线平滑禁止，1 为曲线平滑使能
    settings.curvePress    = ui->curvePressComboBox->currentIndex();       // 压力辅助：0 为饱和蒸汽辅助禁用，1 为饱和蒸汽辅助使能
    settings.buDian        = ui->buDianComboBox->currentIndex();           // 布点开关：0 为分析报告附加点图禁止，1 为分析报告附加点图使能

    // 报告参数
    settings.report1 = ui->report1CheckBox->isChecked(); // 是否选中统计表格报告
    settings.report2 = ui->report2CheckBox->isChecked(); // 是否选中温度 F0 值报告
    settings.report3 = ui->report3CheckBox->isChecked(); // 是否选中综合分析报告
    settings.report4 = ui->report4CheckBox->isChecked(); // 是否选中审计追踪报告

    return settings;
}
