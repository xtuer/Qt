#include "EllipseWidget.h"
#include "ui_EllipseWidget.h"
#include <QPainter>

EllipseWidget::EllipseWidget(QWidget *parent) : QWidget(parent), ui(new Ui::EllipseWidget) {
    ui->setupUi(this);
}

EllipseWidget::~EllipseWidget() {
    delete ui;
}

void EllipseWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(30, 30);

    painter.drawRect(0, 0, 200, 100); // 椭圆的包围矩形
    painter.setBrush(Qt::lightGray);

    painter.drawEllipse(0, 0, 200, 100); // 椭圆
    painter.drawEllipse(230, 0, 100, 100); // 圆
}
