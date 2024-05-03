#include "SlotHome.h"
#include "ui_SlotHome.h"
#include "UiUtil.h"

SlotHome::SlotHome(QWidget *parent): QWidget(parent), ui(new Ui::SlotHome) {
    ui->setupUi(this);
    ui->statusWidget->layout()->setAlignment(Qt::AlignCenter);
    UiUtil::setWidgetPaddingAndSpacing(ui->statusWidget, 30, 0);

    handleEvents();

    // 错误状态时设置 property error 为 true，则会更新界面的样式为红色的。
    ui->statusWidget->setProperty("error", true);
}

SlotHome::~SlotHome() {
    delete ui;
}

void SlotHome::handleEvents() {
    connect(ui->editSterilizationParamsButton, &QToolButton::clicked, this, &SlotHome::emitEditSterilizationParams);
    connect(ui->currentResultButton, &QToolButton::clicked, this, &SlotHome::emitCurrentResult);
    connect(ui->historyResultButton, &QToolButton::clicked, this, &SlotHome::emitHistoryResult);
}
