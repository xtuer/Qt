#include "ComplicatedPathWidget.h"
#include "ui_ComplicatedPathWidget.h"
#include <QPainter>

ComplicatedPathWidget::ComplicatedPathWidget(QWidget *parent) : QWidget(parent), ui(new Ui::ComplicatedPathWidget) {
    ui->setupUi(this);
}

ComplicatedPathWidget::~ComplicatedPathWidget() {
    delete ui;
}

void ComplicatedPathWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    path.lineTo(100, 0);
    path.cubicTo(200, 0, 100, 50, 200, 100); // 贝塞尔曲线
    path.closeSubpath(); // 画一条线到路径的第一个点，闭合路径
    path.addRect(50, 50, 50, 50); // 加一个矩形

    QPainterPath path2;
    path2.addEllipse(80, 30, 50, 50);
    path = path.subtracted(path2); // 扣掉圆

    painter.translate(20, 20);
    painter.setBrush(Qt::lightGray);
    painter.drawPath(path);

    painter.setBrush(Qt::NoBrush);
    painter.drawRect(path.boundingRect());
}
