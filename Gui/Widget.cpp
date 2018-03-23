#include "Widget.h"
#include "Pandora.h"
#include <QImage>
#include <QDebug>

Widget::Widget(QWidget *parent) : QWidget(parent) {
    this->setAttribute(Qt::WA_StyledBackground); // 启用 QSS
}

Widget::~Widget() {
    qDebug() << "~Widget()";
}

void Widget::paintEvent(QPaintEvent *) {

}
