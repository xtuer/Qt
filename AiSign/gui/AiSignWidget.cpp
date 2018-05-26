#include "ui_AiSignWidget.h"
#include "AiSignWidget.h"

#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>

/*-----------------------------------------------------------------------------|
 |                             AiSignWidgetPrivate                             |
 |----------------------------------------------------------------------------*/

struct AiSignWidgetPrivate {
    AiSignWidgetPrivate(AiSignWidget *aiSignWidget);

    void initializeCamera(AiSignWidget *aiSignWidget); // 初始化摄像头

    // 摄像头
    QCamera *camera;
    QCameraViewfinder *cameraViewfinder;
    QCameraImageCapture *cameraImageCapture;
};

AiSignWidgetPrivate::AiSignWidgetPrivate(AiSignWidget *aiSignWidget) {
    initializeCamera(aiSignWidget); // 初始化摄像头
}

// 初始化摄像头
void AiSignWidgetPrivate::initializeCamera(AiSignWidget *aiSignWidget) {
    camera = new QCamera(aiSignWidget);
    cameraViewfinder = new QCameraViewfinder(aiSignWidget);
    cameraImageCapture = new QCameraImageCapture(camera);
    cameraViewfinder->setProperty("class", "CameraWidget");
    aiSignWidget->ui->cameraContainer->layout()->replaceWidget(aiSignWidget->ui->cameraPlaceholderLabel, cameraViewfinder);
    aiSignWidget->ui->cameraPlaceholderLabel->hide();
    camera->setViewfinder(cameraViewfinder);
    camera->start();
}

/*-----------------------------------------------------------------------------|
 |                                AiSignWidget                                 |
 |----------------------------------------------------------------------------*/
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

    d = new AiSignWidgetPrivate(this);
}

// 信号槽事件处理
void AiSignWidget::handleEvents() {

}
