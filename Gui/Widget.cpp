#include "Widget.h"
#include "ui_Widget.h"
#include "FoxWidget.h"

#include <QPainter>
#include <QFont>
#include <QDebug>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    ui->label->setFont(QFont("Monaco", 30));
}

Widget::~Widget() {
    delete ui;
}

void Widget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
}
