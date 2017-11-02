#include "Widget.h"
#include "ui_Widget.h"
#include <QSignalMapper>
#include <QDebug>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    QSignalMapper *mapper = new QSignalMapper(this);
    mapper->setMapping(ui->startButton, "B1");
    mapper->setMapping(ui->stopButton,  "B2");

    connect(ui->startButton, &QPushButton::clicked, mapper, QOverload<>::of(&QSignalMapper::map));
    connect(ui->stopButton,  &QPushButton::clicked, mapper, QOverload<>::of(&QSignalMapper::map));

    connect(mapper, QOverload<const QString &>::of(&QSignalMapper::mapped), [this](const QString &text) {
        qDebug() << sender();
        qDebug() << text;
    });
}

Widget::~Widget() {
    delete ui;
}
