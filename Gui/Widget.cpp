#include "Widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent) {
    this->setAttribute(Qt::WA_StyledBackground); // 启用 QSS
    this->setStyleSheet("border: 2px solid red; background: pink; border-radius: 10px;");
}

Widget::~Widget() {
}
