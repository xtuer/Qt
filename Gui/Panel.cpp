#include "Panel.h"
#include <QPainter>
#include <QtMath>
#include <QPainterPath>

Panel::Panel(QWidget *parent) : QWidget(parent) {

}

//void Panel::paintEvent(QPaintEvent *) {
//    QPainter painter(this);
//    painter.setRenderHint(QPainter::Antialiasing);

//    painter.translate(10, 150);
//    painter.setPen(QPen(Qt::gray, 2));
//    painter.drawLine(0, 0, 628, 0);
//    painter.setPen(QPen(Qt::black, 2));

//    qreal prex = 0, prey = 0;

//    // 0 到 314 为归一为 0 到 M_PI
//    for (int i = 0; i <= 628; ++i) {
//        qreal x = i;
//        qreal y = qSin(i / 314.0 * M_PI) * 100;

//        painter.drawLine(prex, prey, x, y);

//        prex = x;
//        prey = y;
//    }
//}

void Panel::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.translate(10, 150);
    painter.setPen(QPen(Qt::gray, 1, Qt::DashLine));
    painter.drawLine(0, 0, 628, 0);
    painter.setPen(QPen(Qt::black, 1));

    QPainterPath path;
    path.moveTo(0, 0);

    // 0 到 314 为归一为 0 到 M_PI
    for (int i = 0; i <= 628; ++i) {
        qreal x = i;
        qreal y = qSin(i / 314.0 * M_PI) * 100;
        path.lineTo(x, y);
    }

    painter.drawPath(path);
}
