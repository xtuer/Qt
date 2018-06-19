#include "ReportWizardWidget.h"
#include "ui_ReportWizardWidget.h"

#include <QDebug>
#include <QIntValidator>
#include <QDoubleValidator>

// 设备类型的对应参数是否可选：0 为可选，1 为必须
// criterionComboBox 中的一个 item 对应一行，每行 8 个值
static const int CRITERION_RULES[] = {
    0, 0, 1, 0, 0, 1, 0, 1,
    0, 1, 0, 1, 0, 1, 1, 0,
    1, 0, 0, 0, 0, 0, 1, 0,
    1, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 1, 0, 0, 1, 0, 1
};

ReportWizardWidget::ReportWizardWidget(QWidget *parent) : QWidget(parent), ui(new Ui::ReportWizardWidget) {
    initialize();
    handleEvents();

    ui->deviceTypeComboBox->setCurrentIndex(0); // 选择第一个设备
}

ReportWizardWidget::~ReportWizardWidget() {
    delete ui;
}

// 初始化
void ReportWizardWidget::initialize() {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);

    // 隐藏保存和上一步按钮
    ui->saveButton->hide();
    ui->previousButton->setEnabled(false);
    ui->stackedWidget->setCurrentIndex(0);

    ui->killTempLineEdit->setValidator(new QDoubleValidator(this));
    ui->killTimeLineEdit->setValidator(new QIntValidator(this));
}

// 信号槽事件处理
void ReportWizardWidget::handleEvents() {
    // 选择设备类型后显示对应的参数
    connect(ui->deviceTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this] (int index) {
        // 如果选择环境试验设备，则隐藏第一页的各种测量参数和第三页的灭菌参数设置，否则显示它们
        QList<QCheckBox*> cbs = ui->deviceTypesGroupBox->findChildren<QCheckBox*>();

        for (QCheckBox *cb : cbs) {
            cb->setVisible(0 != index);
        }

        ui->mieJunParametersFrame->setVisible(0 != index);

        // 修改测量参数的文本
        int start = index * 8;
        ui->timeZoneCheckBox->setText(textWithRule(ui->timeZoneCheckBox->text(), CRITERION_RULES[start + 0])); // 灭菌时段
        ui->killTimeCheckBox->setText(textWithRule(ui->killTimeCheckBox->text(), CRITERION_RULES[start + 1])); // 测量时间
        ui->averageCheckBox->setText(textWithRule(ui->averageCheckBox->text(),   CRITERION_RULES[start + 2])); // 测量均值
        ui->maxCheckBox->setText(textWithRule(ui->maxCheckBox->text(),           CRITERION_RULES[start + 3])); // 最大值
        ui->minCheckBox->setText(textWithRule(ui->minCheckBox->text(),           CRITERION_RULES[start + 4])); // 最小值
        ui->bodongCheckBox->setText(textWithRule(ui->bodongCheckBox->text(),     CRITERION_RULES[start + 5])); // 波动度
        ui->valueF0CheckBox->setText(textWithRule(ui->valueF0CheckBox->text(),   CRITERION_RULES[start + 6])); // 测量 F0 值
        ui->valueA0CheckBox->setText(textWithRule(ui->valueA0CheckBox->text(),   CRITERION_RULES[start + 7])); // 测量 A0 值
    });

    // 上一步
    connect(ui->previousButton, &QPushButton::clicked, [this] {
        int current = ui->stackedWidget->currentIndex();

        if (current > 0) {
            ui->stackedWidget->setCurrentIndex(--current);
        }

        ui->previousButton->setDisabled(current == 0);
        ui->nextButton->setEnabled(true);
        ui->saveButton->hide();
    });

    // 下一步
    connect(ui->nextButton, &QPushButton::clicked, [this] {
        int current = ui->stackedWidget->currentIndex();
        int count = ui->stackedWidget->count();

        if (current < count) {
            ui->stackedWidget->setCurrentIndex(++current);
        }

        ui->previousButton->setEnabled(true);
        ui->nextButton->setDisabled(current == count-1);
        ui->saveButton->setVisible(current == count-1);
    });

    // 保存设置
    connect(ui->saveButton, &QPushButton::clicked, [this] {
        // 1. 获取设置
        // 2. 保存设置到数据库
        // 3. 发射设置改变的信号
        ReportSettings settings = extractReportSettings();
        qDebug().noquote() << settings.toJson();
        qDebug().noquote() << ReportSettings(settings.toJson()).toJson();

        emit reportSettingsChanged(settings);
    });
}

// 如果 rule 为 0，返回的字符串以 (可) 开头，否则以 (必) 开头
QString ReportWizardWidget::textWithRule(const QString &text, int rule) const {
    int pos = text.indexOf(")");
    QString frog   = text.mid(pos + 1);
    QString prefix = (rule == 0) ? "(可)" : "(必)";

    return prefix + frog;
}

// 获取报表设置
ReportSettings ReportWizardWidget::extractReportSettings() const {
    ReportSettings settings;

    // 设备类型
    settings.deviceType      = ui->deviceTypeComboBox->currentIndex(); // 设备类型
    settings.deviceTypeName  = ui->deviceTypeComboBox->currentText();  // 设备类型名称

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
