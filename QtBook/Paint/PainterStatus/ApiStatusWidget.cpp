#include "ApiStatusWidget.h"
#include "ui_ApiStatusWidget.h"
#include <QPainter>

ApiStatusWidget::ApiStatusWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::ApiStatusWidget) {
    ui->setupUi(this);
}

ApiStatusWidget::~ApiStatusWidget() {
    delete ui;
}

void ApiStatusWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    // [*] 保存 QPainter 的状态
    painter.save();

    // 移动坐标轴
    painter.translate(100, 100);

    // 绘制坐标轴所在的线
    painter.drawLine(-100, 0, 100, 0);
    painter.drawLine(0, -100, 0, 100);
    painter.rotate(45);
    painter.setPen(Qt::red);
    painter.setBrush(Qt::blue);
    painter.setFont(QFont("Monaco", 30));
    painter.drawRect(-50, -50, 100, 100);
    painter.drawText(0, 0, "Hello");

    // [*] 恢复 QPainter 的状态
    painter.restore();

    painter.drawRect(250, 50, 100, 100);
    painter.drawText(250, 50, "Hello");
}
