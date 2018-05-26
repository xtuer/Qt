#include "ui_AiSignWidget.h"
#include "AiSignWidget.h"

AiSignWidget::AiSignWidget(QWidget *parent) : QWidget(parent), ui(new Ui::AiSignWidget) {
    ui->setupUi(this);
    initialize();
    handleEvents();
}

AiSignWidget::~AiSignWidget() {
    delete ui;
}

// 初始化
void AiSignWidget::initialize() {
    setAttribute(Qt::WA_StyledBackground);
    ui->cameraContainer->layout()->setAlignment(ui->captureAndUploadButton, Qt::AlignHCenter); // 按钮居中
}

// 信号槽事件处理
void AiSignWidget::handleEvents() {

}
