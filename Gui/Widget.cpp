#include "Widget.h"
#include "ui_Widget.h"
#include <QSignalMapper>
#include <QDebug>
#include <QKeyEvent>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

}

Widget::~Widget() {
    delete ui;
}

