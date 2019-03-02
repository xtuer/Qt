#include "GridLine.h"
#include <QMouseEvent>

GridLine::GridLine(Qt::Orientation orientation, QWidget *parent) : QWidget(parent), orientation(orientation) {
    int s = 4;

    if (Qt::Horizontal == orientation) {
        setMinimumHeight(s);
        setMaximumHeight(s);
        setStyleSheet("GridLine { border-top: 1px solid #666 } GridLine:hover { border-top-color: orange }"); // 背景
    } else if (Qt::Vertical == orientation) {
        setMinimumWidth(s);
        setMaximumWidth(s);
        setStyleSheet("GridLine { border-left: 1px solid #666 } GridLine:hover { border-left-color: orange }"); // 背景
    }

    setAttribute(Qt::WA_StyledBackground);

    // 点击右键删除线
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, [this]() {
        deleteLater();
    });
}

// 鼠标按下时记录此时鼠标的全局坐标和窗口左上角的坐标
void GridLine::mousePressEvent(QMouseEvent *event) {
    mousePressedPosition = event->globalPos();
    topLeftBeforeMoving  = geometry().topLeft();
}

// 鼠标放开时设置 mousePressedPosition 为空坐标
void GridLine::mouseReleaseEvent(QMouseEvent *) {
    mousePressedPosition = QPoint();
}

// 鼠标移动时如果 mousePressedPosition 不为空，则说明需要移动窗口
// 鼠标移动的位移差，就是窗口移动的位移差
void GridLine::mouseMoveEvent(QMouseEvent *event) {
    if (!mousePressedPosition.isNull()) {
        QPoint delta = event->globalPos() - mousePressedPosition;

        if (Qt::Horizontal == orientation) {
            // 水平线只能上下移动
            move(topLeftBeforeMoving.x(), topLeftBeforeMoving.y() + delta.y());
        } else if (Qt::Vertical == orientation) {
            // 垂直线只能左右移动
            move(topLeftBeforeMoving.x() + delta.x(), topLeftBeforeMoving.y());
        }
    }
}
