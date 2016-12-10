#include "AnimationAlongPathWidget.h"

#include <QPainter>
#include <QtGlobal>
#include <QTimerEvent>

AnimationAlongPathWidget::AnimationAlongPathWidget(QWidget *parent) : QWidget(parent) {
    step = 0.02;
    percent = 0;

    // 构造一个任意的曲线
    path.cubicTo(50, 0, 30, 100, 100, 100);
    path.cubicTo(150, 100, 250, 0, 200, 100);
    path.cubicTo(150, 100, 250, 0, 300, 140);
    path.quadTo(150, 310, 150, 100);

    // 启动定时器
    timerId = startTimer(60);
}

AnimationAlongPathWidget::~AnimationAlongPathWidget() {
}

void AnimationAlongPathWidget::timerEvent(QTimerEvent *event) {
    // 不停的更新 percent 的值，刷新界面，实现动画效果
    if (event->timerId() == timerId) {
        percent += step;

        if (percent < 0 || percent > 1) {
            step = -step; // 运动方向取反
            percent = qMin(percent, 1.0F);
            percent = qMax(percent, 0.0F);
        }

        update();
    }
}

void AnimationAlongPathWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.translate(20, 20);
    painter.drawPath(path);

    painter.setBrush(Qt::red);
    painter.drawEllipse(path.pointAtPercent(percent), 4, 4);
}
