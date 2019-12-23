#include "Widget.h"
#include "ui_Widget.h"

#include <QThread>
#include <QCoreApplication>
#include <QPainter>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
}

Widget::~Widget() {
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    painter.translate(width() / 2, height() / 2);
    painter.setPen(Qt::NoPen);

    int radius = 130;
    painter.setBrush(Qt::magenta);
    painter.drawEllipse(-radius, -radius, radius*2, radius*2);

    radius = 100;
    painter.setBrush(Qt::gray);
    painter.drawEllipse(-radius, -radius, radius*2, radius*2);

    radius = 50;
    painter.setBrush(Qt::yellow);
    painter.drawEllipse(-radius, -radius, radius*2, radius*2);
}
