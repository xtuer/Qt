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

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    loadQss();


}

Widget::~Widget() {
    delete ui;
}

void Widget::loadQss() {
    QAction *action = new QAction();
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
    this->addAction(action);

    connect(action, &QAction::triggered, [] {
        UiUtil::loadQss();
    });
}



