#include <QApplication>
#include <QDebug>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QImage>
#include <QPixmap>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setStyleSheet("QCameraViewfinder, QLabel { border: 2px dashed grey;}");
    QCamera *camera = new QCamera(); // 摄像头对象
    QCameraViewfinder *viewfinder = new QCameraViewfinder(); // 用于实时显示摄像头图像
    QCameraImageCapture *imageCapture = new QCameraImageCapture(camera); // 用于截取摄像头图像
    camera->setViewfinder(viewfinder); // camera 使用 viewfinder 实时显示图像
    viewfinder->setAttribute(Qt::WA_StyledBackground, true); // 使 viewfinder 能够使用 QSS

    QLabel *previewLabel = new QLabel(""); // 拍照预览的 label
    QPushButton *captureButton = new QPushButton("拍照"); // 点击拍照的按钮
    viewfinder->setFixedHeight(150);
    previewLabel->setFixedHeight(150);
    previewLabel->setMinimumWidth(150);
    previewLabel->setAlignment(Qt::AlignCenter);

    // 布局 widgets
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(viewfinder);
    layout->addWidget(previewLabel);
    layout->addWidget(captureButton);
    layout->addStretch();

    QWidget *window = new QWidget();
    window->setLayout(layout);

    QLabel *mask = new QLabel(window);
    mask->setGeometry(60, 70, 90, 60);
    mask->setStyleSheet("background: #99FFFFFF; border-radius: 6px;");

    window->show();
    camera->start();

    // 点击拍照
    QObject::connect(captureButton, &QPushButton::clicked, [=] {
        imageCapture->capture("capture.jpg"); // 如果不传入截图文件的路径，则会使用默认的路径，每次截图生成一个图片
    });
    // 拍照完成后的槽函数，可以把 image 保存到自己想要的位置
    QObject::connect(imageCapture, &QCameraImageCapture::imageCaptured, [=](int id, const QImage &image) {
        Q_UNUSED(id)
        QImage scaledImage = image.scaled(previewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        previewLabel->setPixmap(QPixmap::fromImage(scaledImage));
    });
    return a.exec();
}
