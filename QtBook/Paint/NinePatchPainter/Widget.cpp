#include "Widget.h"
#include "NinePatchPainter.h"

#include <QPixmap>
#include <QPainter>

Widget::Widget(QWidget *parent) : QWidget(parent) {
    pixmap.load(":/rounded-rectangle.png"); // 加载背景图
    ninePatchPainter = new NinePatchPainter(pixmap, 25, 25, 25, 25);
}

Widget::~Widget() {
    delete ninePatchPainter;
}

void Widget::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    painter.drawPixmap(20, 20, 300, 200, pixmap); // 普通的拉伸绘制
    ninePatchPainter->paint(&painter, QRect(340, 20, 300, 200)); // 九宫格绘制
}
