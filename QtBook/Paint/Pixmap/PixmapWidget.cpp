#include "PixmapWidget.h"
#include "ui_PixmapWidget.h"
#include <QPainter>

PixmapWidget::PixmapWidget(QWidget *parent) : QWidget(parent), ui(new Ui::PixmapWidget) {
    ui->setupUi(this);
}

PixmapWidget::~PixmapWidget() {
    delete ui;
}

void PixmapWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPixmap pixmap(":/img/Butterfly.png"); // 从资源文件读取 pixmap

    painter.drawPixmap(20, 20, pixmap); // 按原始尺寸绘制 pixmap
    painter.drawPixmap(225, 20, 250, 159, pixmap); // 缩放绘制 pixmap
    painter.drawPixmap(20, 133, pixmap, 128, 0, 57, 46); // 绘制 pixmap 的一部分

    painter.translate(0, 199);
    painter.drawTiledPixmap(0, 0, width(), height(), pixmap);
}
