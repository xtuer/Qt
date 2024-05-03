#include "SlotParams.h"
#include "ui_SlotParams.h"
#include "UiUtil.h"
#include <QGridLayout>

SlotParams::SlotParams(QWidget *parent): QWidget(parent), ui(new Ui::SlotParams) {
    initUi();
    handleEvents();
}

SlotParams::~SlotParams() {
    delete ui;
}

// 设置灭菌参数，显示到界面上。
void SlotParams::setParams(const QString param) {
    ui->lineEdit_5->setText(param);
}

void SlotParams::initUi() {
    ui->setupUi(this);

    setAttribute(Qt::WA_StyledBackground);
}

void SlotParams::handleEvents() {
    connect(ui->backButton, &QPushButton::clicked, this, &SlotParams::emitBack);
}
