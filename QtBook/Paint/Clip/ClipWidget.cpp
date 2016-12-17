#include "ClipWidget.h"

#include <QPainter>
#include <QPixmap>
#include <QPainterPath>

ClipWidget::ClipWidget(QWidget *parent) : QWidget(parent) {
    resize(300, 150);
}

ClipWidget::~ClipWidget() {
}

void ClipWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.translate(width()/2, height()/2);
    painter.setPen(QPen(Qt::gray, 4));

    QPixmap pixmap("/Users/Biao/Pictures/dog.png");

    // [1.1] 创建圆的 path
    QPainterPath circlePath;
    circlePath.addEllipse(-50, -50, 100, 100);

    painter.translate(-60, 0);
    painter.setClipPath(circlePath); // [1.2] 指定 clip 为圆的 path
    painter.drawPixmap(circlePath.boundingRect().toRect(), pixmap); // [1.3] 绘制出来的 pixmap 只会显示在圆中的部分
    painter.drawPath(circlePath);

    // 同上
    // [2.1] 创建星形的 path
    QPainterPath starPath;
    starPath.moveTo(0, -50);
    starPath.lineTo(20, -20);
    starPath.lineTo(50, 0);
    starPath.lineTo(20, 20);
    starPath.lineTo(0, 50);
    starPath.lineTo(-20, 20);
    starPath.lineTo(-50, 0);
    starPath.lineTo(-20, -20);
    starPath.closeSubpath();

    painter.translate(120, 0);
    painter.setClipPath(starPath); // [2.2] 指定 clip 为星形的 path
    painter.drawPixmap(starPath.boundingRect().toRect(), pixmap); // [2.3] 绘制出来的 pixmap 只会显示在星形中的部分
    painter.drawPath(starPath);
}
