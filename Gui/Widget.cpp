#include "Widget.h"
#include "ui_Widget.h"
#include "FoxWidget.h"

#include <QPainter>
#include <QFont>
#include <QDebug>
#include <QKeySequence>
#include <QShortcut>

RoundButtonWidget::RoundButtonWidget(QWidget *parent) : QWidget(parent), ui(new Ui::RoundButtonWidget) {
    ui->setupUi(this);

//    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_L), this);

//    connect(shortcut, &QShortcut::activated, [] {
//        qDebug() << "shortcut";
//    });
}

RoundButtonWidget::~RoundButtonWidget() {
    delete ui;
}

void RoundButtonWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
}
