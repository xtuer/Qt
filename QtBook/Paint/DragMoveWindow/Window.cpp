#include "Window.h"
#include <QMouseEvent>

Window::Window(QWidget *parent) : QWidget(parent) {
}

Window::~Window() {

}

// 鼠标按下时记录此时鼠标的全局坐标和窗口左上角的坐标
void Window::mousePressEvent(QMouseEvent *event) {
    mousePressedPosition = event->globalPos();
    windowPositionBeforeMoving = frameGeometry().topLeft();
}

// 鼠标放开时设置 mousePressedPosition 为空坐标
void Window::mouseReleaseEvent(QMouseEvent *) {
    mousePressedPosition = QPoint();
}

// 鼠标移动时如果 mousePressedPosition 不为空，则说明需要移动窗口
// 鼠标移动的位移差，就是窗口移动的位移差
void Window::mouseMoveEvent(QMouseEvent *event) {
    if (!mousePressedPosition.isNull()) {
        QPoint delta = event->globalPos() - mousePressedPosition;
        QPoint newPosition = windowPositionBeforeMoving + delta;
        move(newPosition);
    }
}
