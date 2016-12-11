#include "PaintButtonWidget.h"
#include "ui_PaintButtonWidget.h"
#include <QPainter>

PaintButtonWidget::PaintButtonWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::PaintButtonWidget) {
    ui->setupUi(this);
}

PaintButtonWidget::~PaintButtonWidget() {
    delete ui;
}

void PaintButtonWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::transparent);
    painter.fillRect(0, 0, width() << 1, height() << 1, Qt::darkGray);

    // 移动坐标轴原点到窗口的中心，这样好计算一些
    painter.translate(width() >> 1, height() >> 1);

    int radius = 100;

    // 1. 外边框
    QLinearGradient lg1(0, -radius, 0, radius);
    lg1.setColorAt(0.0, QColor(255, 255, 255));
    lg1.setColorAt(1.0, QColor(166, 166, 166));
    painter.setBrush(lg1);
    painter.drawEllipse(-radius, -radius, radius << 1, radius << 1);

    // 2. 内边框
    radius -= 13;
    QLinearGradient lg2(0, -radius, 0, radius);
    lg2.setColorAt(0.0, QColor(155, 155, 155));
    lg2.setColorAt(1.0, QColor(255, 255, 255));
    painter.setBrush(lg2);
    painter.drawEllipse(-radius, -radius, radius << 1, radius << 1);

    // 3. 内部的圆
    radius -= 4;
    QRadialGradient rg(0, 0, radius);
    rg.setColorAt(0.0, QColor(245, 0, 0));
    rg.setColorAt(0.6, QColor(210, 0, 0));
    rg.setColorAt(1.0, QColor(140, 0, 0));
    painter.setBrush(rg);
    painter.drawEllipse(-radius, -radius, radius << 1, radius << 1);

    // 4. 高光
    QPainterPath smallCircle;
    QPainterPath bigCircle;
    radius -= 3;
    smallCircle.addEllipse(-radius, -radius - 2, radius << 1, radius << 1);
    radius *= 2;
    bigCircle.addEllipse(-radius, -radius + 140, radius << 1, radius << 1);

    // 高光的形状为小圆扣掉大圆的部分
    QPainterPath highlight = smallCircle - bigCircle;

    QLinearGradient lg3(0, -radius / 2, 0, 0);
    lg3.setColorAt(0.0, QColor(255, 255, 255, 220));
    lg3.setColorAt(1.0, QColor(255, 255, 255, 30));
    painter.setBrush(lg3);
    painter.rotate(-20);
    painter.drawPath(highlight);
}
