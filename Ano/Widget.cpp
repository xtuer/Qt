#include "Widget.h"
#include "ui_Widget.h"

#include <QThread>
#include <QCoreApplication>
#include <QPainter>
#include <QPainterPath>
#include <QtMath>
#include <QDebug>
#include <QStringListModel>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    QStringListModel *model = new QStringListModel();
    QStringList names;

    for (int i = 0; i < 100; i++) {
        names << QString::number(i);
    }
    model->setStringList(names);
    ui->listView->setModel(model);

    ui->listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(ui->listView->verticalScrollBar(), &QAbstractSlider::rangeChanged, [=](int min, int max) {
        qDebug() << min << max;
        ui->verticalScrollBar->setRange(min, max);
    });

    connect(ui->listView->verticalScrollBar(), &QAbstractSlider::valueChanged, [=](int value) {
        ui->verticalScrollBar->setValue(value);
    });

    connect(ui->verticalScrollBar, &QAbstractSlider::valueChanged, [=](int value) {
        ui->listView->verticalScrollBar()->setValue(value);
    });
}

Widget::~Widget() {
    delete ui;
}

void Widget::paintEvent(QPaintEvent *) {

}
