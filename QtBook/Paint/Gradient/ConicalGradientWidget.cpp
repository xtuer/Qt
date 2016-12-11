#include "ConicalGradientWidget.h"
#include "ui_ConicalGradientWidget.h"
#include <QPainter>
#include <QConicalGradient>

ConicalGradientWidget::ConicalGradientWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::ConicalGradientWidget) {
    ui->setupUi(this);
}

ConicalGradientWidget::~ConicalGradientWidget() {
    delete ui;
}

void ConicalGradientWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);

    qreal startAngle = 45; // 渐变开始的角度
    QPointF center(0, 0);  // 渐变的中心

    QConicalGradient gradient(center, startAngle);
    gradient.setColorAt(0.0, Qt::red);
    gradient.setColorAt(0.33, Qt::green);
    gradient.setColorAt(0.66, Qt::blue);
    gradient.setColorAt(1.0, Qt::red);

    painter.setPen(Qt::darkGray);
    painter.setBrush(gradient);
    painter.drawEllipse(center, 150, 150);
}
