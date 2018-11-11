#include "ReportWizard.h"
#include "ui_ReportWizard.h"
#include "ReportWizardBase.h"
#include "bean/ReportSettings.h"

// 引入报告设置的 Widget
#include "DGJBX.h"
#include "ZQMJ.h"

#include <QDebug>

ReportWizard::ReportWizard(QWidget *parent) : QWidget(parent), ui(new Ui::ReportWizard) {
    initialize();
    handleEvents();

    ui->deviceListWidget->setCurrentRow(0); // 显示第一个设备的设置
}

ReportWizard::~ReportWizard() {
    delete ui;
}

// 从数据库中加载设备的报告设置
ReportSettings ReportWizard::loadReportSettings(int deviceType) {
    qDebug() << "读取设备报告设置" << deviceType;
    // TODO: 使用以前的逻辑读取, 取消下面的注释即可
    // ReportSettings settings = ReportSettingsDao::findReportSettingsByDeviceType(deviceType);
    // return settings;
    return ReportSettings();
}

// 保存设备的报告设置到数据库
void ReportWizard::saveReportSettings(const ReportSettings &settings) {
    qDebug() << "保存设备报告设置" << settings.deviceType;
    // TODO: 使用以前的逻辑保存, 取消下面的注释即可
    // ReportSettingsDao::insertOrUpdateReportSettings(settings);
    // emit reportSettingsChanged(settings);
}

// 初始化
void ReportWizard::initialize() {
    ui->setupUi(this);

    while (ui->stackedWidget->count() > 0) {
        delete ui->stackedWidget->widget(0);
    }

    // TODO: 增加新的设备报告配置 Widget 到 stackedWidget
    ui->stackedWidget->addWidget(new DGJBX(this));
    ui->stackedWidget->addWidget(new ZQMJ(this));
}

// 信号槽处理
void ReportWizard::handleEvents() {
    // 切换设备的配置
    connect(ui->deviceListWidget, &QListWidget::currentRowChanged, [this] (int row) {
        // 1. 显示当前设置的 Widget
        // 2. Widget 跳转到第一页
        // 3. 禁用上一页按钮和启用下一页按钮
        // 4. 隐藏保存按钮
        // 5. 从数据库加载配置, 并显示到设置的 Ui 上

        ui->stackedWidget->setCurrentIndex(row); // [1] 显示当前设置的 Widget (提示: 超过 count 则不会进行设置)

        ReportWizardBase *wizard = getCurrentSettingsWidget();
        if (nullptr == wizard) { return; }

        // [2] Widget 跳转到第一页
        while (wizard->prevPage());

        // [3] 禁用上一页按钮和启用下一页按钮
        // [4] 隐藏保存按钮
        ui->prevPageButton->setEnabled(false);
        ui->nextPageButton->setEnabled(true);
        ui->saveButton->hide();

        // [5] 从数据库加载配置, 并显示到设置的 Ui 上
        ReportSettings settings = loadReportSettings(row);
        wizard->setReportSettings(settings);
    });

    // 点击上一页按钮, 切换设备配置到上一页
    connect(ui->prevPageButton, &QPushButton::clicked, [this] {
        ReportWizardBase *wizard = getCurrentSettingsWidget();

        if (nullptr != wizard) {
            // 没有上一页了禁用上一页按钮
            bool hasPrev = wizard->prevPage();
            ui->prevPageButton->setEnabled(hasPrev);
            ui->nextPageButton->setEnabled(true);
            ui->saveButton->setVisible(false);
        }
    });

    // 点击下一页按钮, 切换设备配置到下一页
    connect(ui->nextPageButton, &QPushButton::clicked, [this] {
        ReportWizardBase *wizard = getCurrentSettingsWidget();

        if (nullptr != wizard) {
            // 没有下一页了禁用下一页按钮, 显示保存按钮
            bool hasNext = wizard->nextPage();
            ui->prevPageButton->setEnabled(true);
            ui->nextPageButton->setEnabled(hasNext);
            ui->saveButton->setVisible(!hasNext);
        }
    });

    // 点击保存按钮, 进行保存
    connect(ui->saveButton, &QPushButton::clicked, [this] {
        // 由于多个设备的设置可能一样, 所以会复用同一个报告向导的类, 所以设备类型需要在这里进行确定
        ReportWizardBase *wizard = getCurrentSettingsWidget();
        ReportSettings settings = wizard->getReportSettings();
        settings.deviceType = ui->stackedWidget->currentIndex();
        saveReportSettings(settings);
    });
}

// 获取当前设备配置的 Widget
ReportWizardBase* ReportWizard::getCurrentSettingsWidget() const {
    return dynamic_cast<ReportWizardBase*>(ui->stackedWidget->currentWidget());
}
