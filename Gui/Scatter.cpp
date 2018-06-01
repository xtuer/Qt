#include "Scatter.h"

#include <QMouseEvent>
#include <QDebug>
#include <QAction>
#include <QMenu>

Scatter::Scatter(double xRatio, double yRatio, QWidget *parent) : QWidget(parent), xRatio(xRatio), yRatio(yRatio) {
    setAttribute(Qt::WA_StyledBackground);

    // 创建右键菜单
    setContextMenuPolicy(Qt::CustomContextMenu);
    QAction *deleteAction = new QAction("删除", this);

    connect(deleteAction, &QAction::triggered, [this] {
        emit aboutRemove(this);
    });

    connect(this, &QWidget::customContextMenuRequested, [=](const QPoint &pos) {
        QMenu menu;
        menu.addAction(deleteAction);
        menu.exec(mapToGlobal(pos));
    });
}

void Scatter::setPosition(double xRatio, double yRatio) {
    this->xRatio = xRatio;
    this->yRatio = yRatio;
    updatePosition();
}

// 更新坐标
void Scatter::updatePosition() {
    int x = parentWidget()->width() * xRatio;
    int y = parentWidget()->height() * yRatio;
    move(x, y);
}

// 鼠标按下时记录此时鼠标的全局坐标和 widget 左上角的坐标
void Scatter::mousePressEvent(QMouseEvent *event) {
    mousePressedPosition = event->globalPos();
    topLeftPositionBeforeMoving = geometry().topLeft();
}

// 鼠标放开时设置 mousePressedPosition 为空坐标
void Scatter::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event)
    mousePressedPosition = QPoint();
}

// 鼠标移动时如果 mousePressedPosition 不为空，则说明需要移动 widget
// 鼠标移动的位移差，就是 widget移动的位移差
void Scatter::mouseMoveEvent(QMouseEvent *event) {
    if (!mousePressedPosition.isNull()) {
        QPoint delta = event->globalPos() - mousePressedPosition;
        QPoint newPosition = topLeftPositionBeforeMoving + delta;

        // 但位置变化后，更新在父 widget 中的 xRatio 和 yRatio
        int x = newPosition.x();
        int y = newPosition.y();
        int tw = width();
        int th = height();
        int pw = parentWidget()->width();
        int ph = parentWidget()->height();
        x = qMax(0, x);
        x = qMin(x, pw-tw);
        y = qMax(0, y);
        y = qMin(y, ph-th);

        this->xRatio = (double) x / pw;
        this->yRatio = (double) y / ph;
        move(x, y);
    }
}
