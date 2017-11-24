#include "Widget.h"
#include "ui_Widget.h"
#include <QSignalMapper>
#include <QDebug>
#include <QPainter>
#include <QFont>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

}

Widget::~Widget() {
    delete ui;
}

void Widget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    QFont backFont = QFont("HelveticaNeue-Light", 24, 1);
    QFont frontFont = QFont("HelveticaNeue-Light", 24, 600);
    frontFont.setBold(true);

    painter.setFont(frontFont);
    painter.setPen(Qt::red);
    painter.drawText(10, 30, "去你大爷的");

    painter.setFont(backFont);
    painter.setPen(Qt::white);
    painter.drawText(10, 30, "去你大爷的");
}
