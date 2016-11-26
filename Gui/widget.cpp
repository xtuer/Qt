#include "widget.h"
#include "ui_widget.h"
#include "UiUtil.h"

#include <QDebug>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QThread>
#include <QDir>
#include <QFile>
#include <QPaintEvent>
#include <QPainterPath>
#include <QPainter>
#include <QStandardItemModel>
#include <QAction>
#include <QModelIndex>
#include <QToolTip>
#include <QPainter>
#include <QPixmap>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    loadQss();


}

Widget::~Widget() {
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPixmap pixmap("/Users/Biao/Desktop/tu1.jpg");
    pixmap.setDevicePixelRatio(2);

    painter.drawPixmap(0, 0, pixmap);
}

void Widget::loadQss() {
    QAction *action = new QAction();
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    this->addAction(action);

    connect(action, &QAction::triggered, [] {
        UiUtil::loadQss();
    });
}




