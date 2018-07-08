#include "Widget.h"
#include "ui_Widget.h"
#include <QWidget>
#include <QPushButton>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);

    ui->pushButton->parentWidget()->layout()->setAlignment(ui->pushButton, Qt::AlignCenter);
}

Widget::~Widget() {
    delete ui;
}
