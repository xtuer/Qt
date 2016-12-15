#include "Widget.h"
#include "ui_Widget.h"
#include <QPainter>
#include <QDebug>
#include "Foo.h"
#include "Bar.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    xtuer::Foo *f = new xtuer::Foo();
    boo::Bar *b = new boo::Bar();

    connect(ui->slotButton, &QPushButton::clicked, f, &xtuer::Foo::bar);
    connect(f, &xtuer::Foo::hello, b, &boo::Bar::say);

    connect(ui->signalButton, &QPushButton::clicked, [=] {
        f->say();
    });
}

Widget::~Widget() {
    delete ui;
}

void Widget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
}
