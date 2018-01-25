#include "Widget.h"
#include "Pandora.h"
#include <QImage>

Widget::Widget(QWidget *parent) : QWidget(parent) {
    this->setAttribute(Qt::WA_StyledBackground); // 启用 QSS
}

Widget::~Widget() {
}

void Widget::paintEvent(QPaintEvent *) {

}

int magic = 12;
