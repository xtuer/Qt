#include "Form.h"
#include "ui_Form.h"
#include <Lib-1/Config.h>
#include <QDebug>

Form::Form(QWidget *parent) : QWidget(parent), ui(new Ui::Form) {
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, [] {
        qDebug() << &Config::instance();
        qDebug() << ++Config::count;
        qDebug() << ++g;
    });
}

Form::~Form() {
    delete ui;
}
