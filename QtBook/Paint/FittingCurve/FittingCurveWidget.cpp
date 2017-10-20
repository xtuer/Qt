#include "FittingCurveWidget.h"
#include <QPainter>
#include <QtMath>
#include <QPainterPath>

FittingCurveWidget::FittingCurveWidget(QWidget *parent) : QWidget(parent) {
    setWindowTitle("小线段拟合曲线");
}

void FittingCurveWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(10, 150);

    // 绘制坐标轴
    painter.setPen(QPen(Qt::gray, 1, Qt::DashLine));
    painter.drawLine(0, 0, 700, 0);
    painter.drawLine(0, -200, 0, 200);

    // 计算正弦的坐标点
    QPainterPath path(QPointF(0, 0));

    // [0, 314] 归一为 [0, PI]
    for (int i = 0; i <= 628; ++i) {
        qreal x = i;
        qreal y = qSin(i/314.0*M_PI) * 100;
        path.lineTo(x, y);
    }

    painter.setPen(QPen(Qt::black, 1));
    painter.drawPath(path);
}

//void FittingCurveWidget::paintEvent(QPaintEvent *) {
//    QPainter painter(this);
//    painter.setRenderHint(QPainter::Antialiasing);

//    painter.translate(10, 150);
//    painter.setPen(QPen(Qt::gray, 1, Qt::DashLine));
//    painter.drawLine(0, 0, 700, 0);
//    painter.drawLine(0, -200, 0, 200);
//    painter.setPen(QPen(Qt::black, 1));

//    qreal prex = 0, prey = 0;

//    // 0 到 314 为归一为 0 到 M_PI
//    for (int i = 0; i <= 628; ++i) {
//        qreal x = i;
//        qreal y = qSin(i/314.0*M_PI) * 100;

//        painter.drawLine(prex, prey, x, y);

//        prex = x;
//        prey = y;
//    }
//}
