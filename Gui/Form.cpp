#include "Form.h"
#include "ui_Form.h"

#include <QDebug>
#include <QImage>
#include <QMatrix>
#include <QPixmap>
#include <QPainter>

Form::Form(QWidget *parent) : QWidget(parent), ui(new Ui::Form) {
//    ui->setupUi(this);

    pixmap.load("/Users/Biao/Desktop/point.png"); // 40x608
}

void Form::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    int w = width();
    int h = height();

    painter.translate(w / 2, h - 20);
    painter.scale(1, -1);
    painter.drawPixmap(-20, -10, pixmap); // 居中绘制

    painter.save();
    painter.rotate(30);
    painter.drawPixmap(-20, -10, pixmap);
    painter.restore();

    painter.save();
    painter.rotate(-30);
    painter.drawPixmap(-20, -10, pixmap);
    painter.restore();

    painter.setBrush(Qt::black);
    painter.drawRect(-2, -2, 4, 4);
}
