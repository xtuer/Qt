#include "Widget.h"
#include "ui_Widget.h"
#include "CheckedButton.h"
#include <QStyle>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {
    ui->setupUi(this);

    b = new CheckedButton();
    ui->gridLayout->replaceWidget(ui->pushButton, b);
    delete ui->pushButton;

    connect(ui->pushButton_2, &QPushButton::clicked, [&] {
        b->setChecked(true);
    });

    connect(b, &CheckedButton::clicked, [](bool checked) {
        qDebug() << "Checked: " << checked;
    });
}

Widget::~Widget() {
    delete ui;
}
