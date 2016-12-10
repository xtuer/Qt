#include "AntLineWidget.h"
#include <QTimerEvent>
#include <QPainter>
#include <QPen>
#include <QPainterPath>

AntLineWidget::AntLineWidget(QWidget *parent) : QWidget(parent), PATTERN_LENGTH(4) {
    dashes = PATTERN_LENGTH;
    spaces = PATTERN_LENGTH;

    for (int i = 0; i < 400; ++i) {
        dashPattern << PATTERN_LENGTH;
    }

    timerId = startTimer(150);
}

AntLineWidget::~AntLineWidget() {
}

void AntLineWidget::timerEvent(QTimerEvent *event) {
    if (event->timerId() == timerId) {
        advanceDashes();
        update(); // 更好的方式是更新蚂蚁线的所在的范围，而不是整个界面都刷新，用 update(rect)
    }
}

void AntLineWidget::advanceDashes() {
    if (PATTERN_LENGTH == dashes && PATTERN_LENGTH == spaces) {
        dashes = 0;
        spaces = 0;
    }

    if (0 == dashes && spaces < PATTERN_LENGTH) {
        ++spaces;
    } else if (PATTERN_LENGTH == spaces && dashes < PATTERN_LENGTH) {
        ++dashes;
    }

    dashPattern[0] = dashes;
    dashPattern[1] = spaces;
}

void AntLineWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen;
    pen.setDashPattern(dashPattern); // 蚂蚁线的 style

    painter.translate(20, 20);
    painter.setPen(Qt::white);
    painter.drawRect(0, 0, 100, 100);
    painter.setPen(pen);
    painter.drawRect(0, 0, 100, 100);

    QPainterPath path;
    path.cubicTo(50, 0, 50, 100, 100, 100);
    path.cubicTo(150, 0, 150, 100, 200, 0);

    painter.translate(120, 0);
    painter.setPen(Qt::white);
    painter.drawPath(path);

    painter.setPen(pen);
    painter.drawPath(path);
}
