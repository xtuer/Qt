#include "Form.h"
#include "ui_Form.h"
#include <Lib-1/Config.h>
#include <QDebug>

Form::Form(QWidget *parent) : QWidget(parent), ui(new Ui::Form) {
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, [this] {
        qDebug() << &Config::instance();
        ++Config::count;
        ++g;
        ui->pushButton->setText(QString("App-2 (Lib2): count %1, g %2").arg(Config::count).arg(g));
    });
}

Form::~Form() {
    delete ui;
}
