#include "Widget.h"
#include "ui_Widget.h"
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QImage>
#include <QPixmap>
#include <QGridLayout>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    camera = new QCamera(this);
    viewfinder = new QCameraViewfinder(this); // 实时显示摄像头图像
    imageCapture = new QCameraImageCapture(camera); // 用于截取摄像头图像

    viewfinder->setAttribute(Qt::WA_StyledBackground, true);
    viewfinder->setFixedHeight(150);
    ui->previewLabel->setFixedHeight(150);
    ui->previewLabel->setAlignment(Qt::AlignCenter);

    // 视频的 viewfinder 替换占位的 label
    delete layout()->replaceWidget(ui->placeholderLabel, viewfinder);
    delete ui->placeholderLabel;

    camera->setViewfinder(viewfinder);
    camera->start(); // 打开摄像头

    connect(ui->captureButton, &QPushButton::clicked, [=] {
        imageCapture->capture("capture.jpg"); // 截图
    });

    // 截图时的信号槽
    connect(imageCapture, &QCameraImageCapture::imageCaptured, [=](int id, const QImage &image) {
        Q_UNUSED(id)
        QImage scaledImage = image.scaled(ui->previewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->previewLabel->setPixmap(QPixmap::fromImage(scaledImage));

        image.scaled(500, 500, Qt::KeepAspectRatio, Qt::SmoothTransformation).save("x.jpg", "jpg", 50);
    });
}

Widget::~Widget() {
    camera->stop();
    delete ui;
}

