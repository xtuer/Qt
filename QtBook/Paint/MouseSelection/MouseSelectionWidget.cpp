#include "MouseSelectionWidget.h"
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

MouseSelectionWidget::MouseSelectionWidget(QWidget *parent) : QWidget(parent), modifyMode(false) {
}

void MouseSelectionWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawRect(rect);
}

void MouseSelectionWidget::mousePressEvent(QMouseEvent *event) {
    modifyMode = true;
    rect.setTopLeft(event->pos());
}

void MouseSelectionWidget::mouseReleaseEvent(QMouseEvent *) {
    modifyMode = false;
}

void MouseSelectionWidget::mouseMoveEvent(QMouseEvent *event) {
    if (modifyMode) {
        rect.setBottomRight(event->pos());
        update();
    }
}
