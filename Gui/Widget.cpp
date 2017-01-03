#include "Widget.h"
#include "ui_Widget.h"
#include "FoxWidget.h"

#include <QPainter>
#include <QDebug>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    QWidget *w = new FoxWidget();
    w->setWindowFlags(Qt::ToolTip);
    w->resize(400, 300);
//    w->setAttribute(Qt::WA_DeleteOnClose);
    connect(ui->pushButton, &QPushButton::clicked, [=] {
        w->move(QCursor::pos());
        w->show();
    });
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
