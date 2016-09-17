#include "FramelessWindow.h"

#include <QMouseEvent>
#include <QGridLayout>
#include <QGraphicsDropShadowEffect>

struct FramelessWindowPrivate {
    FramelessWindowPrivate(QWidget *contentWidget) : contentWidget(contentWidget) {}

    QWidget *contentWidget;
    QPoint mousePressedPosition; // 鼠标按下时的坐标
    QPoint windowPositionAsDrag; // 鼠标按小时窗口左上角的坐标
};

FramelessWindow::FramelessWindow(QWidget *contentWidget, QWidget *parent) : QWidget(parent) {
    setWindowFlags(Qt::FramelessWindowHint);    // 去掉边框
    setAttribute(Qt::WA_TranslucentBackground); // 背景透明

    d = new FramelessWindowPrivate(contentWidget);

    // 添加阴影
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(contentWidget);
    shadowEffect->setColor(Qt::lightGray);
    shadowEffect->setBlurRadius(4); // 阴影的大小
    shadowEffect->setOffset(0, 0);
    contentWidget->setGraphicsEffect(shadowEffect);

    // 添加到窗口中
    QGridLayout *lo = new QGridLayout();
    lo->addWidget(contentWidget, 0, 0);
    lo->setContentsMargins(4, 4, 4, 4); // 注意和阴影大小的协调
    setLayout(lo);
}

FramelessWindow::~FramelessWindow() {
    delete d;
}

void FramelessWindow::mousePressEvent(QMouseEvent *e) {
    // 记录鼠标按下时全局的位置和窗口左上角的位置
    d->mousePressedPosition = e->globalPos();
    d->windowPositionAsDrag = pos();
}

void FramelessWindow::mouseReleaseEvent(QMouseEvent *e) {
    Q_UNUSED(e)
    // 鼠标放开始设置鼠标按下的位置为 null，表示鼠标没有被按下
    d->mousePressedPosition = QPoint();
}

void FramelessWindow::mouseMoveEvent(QMouseEvent *e) {
    if (!d->mousePressedPosition.isNull()) {
        // 鼠标按下并且移动时，移动窗口, 相对于鼠标按下时的位置计算，是为了防止误差累积
        QPoint delta = e->globalPos() - d->mousePressedPosition;
        move(d->windowPositionAsDrag + delta);
    }
}
