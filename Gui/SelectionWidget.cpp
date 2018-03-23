#include "SelectionWidget.h"
#include <QPainter>

SelectionWidget::SelectionWidget(QWidget *parent) : QWidget(parent) {
    setMinimumSize(200, 200);
}

void SelectionWidget::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setBrush(QColor(255, 0, 0, 20));

    painter.drawRect(rect());
}
