#include "Widget.h"
#include "ui_Widget.h"

#include <QThread>
#include <QCoreApplication>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, [=] {
        for (int i = 0; i < 100; ++i) {
            ui->plainTextEdit->appendPlainText(QString::number(i));
            QCoreApplication::processEvents();
        }
    });
}

Widget::~Widget() {
    delete ui;
}
