#include "ui_AiSignWidget.h"
#include "AiSignWidget.h"

#include <QDebug>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QPixmap>
#include <QImage>

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
    // [Camera] 点击拍照上传按钮进行拍照
    connect(ui->captureAndUploadButton, &QPushButton::clicked, [this] {
        d->cameraImageCapture->capture("capture.jpg"); // 如果不传入截图文件的路径，则会使用默认的路径，每次截图生成一个图片
    });

    // [Camera] 拍照完成后的槽函数，可以把 image 保存到自己想要的位置
    QObject::connect(d->cameraImageCapture, &QCameraImageCapture::imageCaptured, [this](int id, const QImage &image) {
        Q_UNUSED(id)
        // 1. 显示预览图
        // 2. 保存到系统
        // 3. 签到
        QImage previewImage1 = image.scaled(ui->previewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->previewLabel->setPixmap(QPixmap::fromImage(previewImage1));

        QImage previewImage2 = image.scaledToHeight(ui->cameraPictureLabel->size().height(), Qt::SmoothTransformation);
        int x = (previewImage2.width() - ui->cameraPictureLabel->width()) / 2;
        int w = ui->cameraPictureLabel->width();
        int h = ui->cameraPictureLabel->height();
        ui->cameraPictureLabel->setPixmap(QPixmap::fromImage(previewImage2.copy(x, 0, w, h)));
    });
}
