#include "Widget.h"
#include "ui_Widget.h"
#include <QPainter>
#include <QDebug>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    connect(ui->slotButton, SIGNAL(clicked(bool)), this, SLOT(foo()));
}

Widget::~Widget() {
    delete ui;
}

//void Widget::foo() {
//    qDebug() << "foo()";
//}

void Widget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
}
