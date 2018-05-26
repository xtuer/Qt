#include "MyWidget.h"
#include "ui_MyWidget.h"
#include "MessageShower.h"

#include <QDebug>
#include <QDateTime>
#include <QTimer>

MyWidget::MyWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MyWidget) {
    initializeUi();
//    handleEvents();
}

MyWidget::~MyWidget() {
    qDebug() << "~MyWidget()";
    delete ui;
}

void MyWidget::initializeUi() {
    ui->setupUi(this);
    messageShower = new MessageShower(this);
    messageShower->setGeometry(0, 0, 400, 200);
}

void MyWidget::handleEvents() {
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this] {
        messageShower->showMessage(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    });
    timer->start(100);
}

