#include "RadialGradientWidget.h"
#include "ui_RadialGradientWidget.h"
#include <QPainter>
#include <QRadialGradient>

RadialGradientWidget::RadialGradientWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::RadialGradientWidget) {
    ui->setupUi(this);
}

RadialGradientWidget::~RadialGradientWidget() {
    delete ui;
}

void RadialGradientWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);

    qreal radius = 150;    // 半径
    QPointF center(0, 0);  // 圆心
    QPointF focus(80, 30); // 焦点

    // 径向渐变
    QRadialGradient gradient(center, radius, focus);
    gradient.setColorAt(0.0, Qt::red);
    gradient.setColorAt(1.0, Qt::blue);

    // 径向渐变填充圆
    painter.setPen(Qt::darkGray);
    painter.setBrush(gradient);
    painter.drawEllipse(center, radius, radius);

    // 绘制圆心和焦点
    painter.setBrush(Qt::gray);
    painter.drawEllipse(center, 4, 4);
    painter.drawEllipse(focus, 4, 4);
}
