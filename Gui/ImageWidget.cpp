#include "ImageWidget.h"
#include "ui_ImageWidget.h"
#include "ImageEffects.h"
#include <QEventLoop>
#include <QDebug>

ImageWidget::ImageWidget(QWidget *parent) : QWidget(parent), ui(new Ui::ImageWidget) {
    ui->setupUi(this);
    image.load("/Users/Biao/Pictures/ade.jpg");
    pixmap = QPixmap::fromImage(image);

    ui->label->setScaledContents(true);
    ui->label->setPixmap(pixmap);

    connect(ui->comboBox, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), [this] (const QString &text) {
        if ("原始图片" == text) {
            pixmap = QPixmap::fromImage(image);
        } else if ("灰度效果" == text) {
            pixmap = QPixmap::fromImage(ImageEffects::gray(image));
        } else if ("复古效果" == text) {
            pixmap = QPixmap::fromImage(ImageEffects::siepa(image));
        } else if ("红色蒙版" == text) {
            pixmap = QPixmap::fromImage(ImageEffects::red(image));
        } else if ("反转效果" == text) {
            pixmap = QPixmap::fromImage(ImageEffects::invert(image));
        } else if ("变亮图片" == text) {
            pixmap = QPixmap::fromImage(ImageEffects::brightness(image, 70));
        } else if ("变暗图片" == text) {
            pixmap = QPixmap::fromImage(ImageEffects::brightness(image, -70));
        }

        ui->label->setPixmap(pixmap);
    });
}

ImageWidget::~ImageWidget() {
    delete ui;
}

