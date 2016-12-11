#include "CodeStatusWidget.h"
#include "ui_CodeStatusWidget.h"
#include <QPainter>

CodeStatusWidget::CodeStatusWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::CodeStatusWidget) {
    ui->setupUi(this);
}

CodeStatusWidget::~CodeStatusWidget() {
    delete ui;
}

void CodeStatusWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    // [*] 保存 QPainter 的状态
    QPen pen = painter.pen();
    QBrush brush = painter.brush();
    QFont font = painter.font();

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
    painter.rotate(-45);
    painter.translate(-100, -100);
    painter.setFont(font);
    painter.setPen(pen);
    painter.setBrush(brush);

    painter.drawRect(250, 50, 100, 100);
    painter.drawText(250, 50, "Hello");
}
