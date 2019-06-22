#include "MyWidget.h"
#include "ui_MyWidget.h"
#include "MessageShower.h"

#include <QDebug>
#include <QDateTime>
#include <QTimer>
#include <QCloseEvent>

#include <QThread>
#include <QTimer>
#include <QItemSelectionModel>

MyWidget::MyWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MyWidget) {
    initializeUi();
    handleEvents();

    qDebug() << QThread::currentThread();
    thread = new QThread();
    thread->start();
    thread->moveToThread(thread);

    QTimer *timer = new QTimer(this);
    int count = 0;
    connect(timer, &QTimer::timeout, thread, [&] {
        qDebug() << QThread::currentThread();
        qDebug() << thread->isRunning();

        if (++count == 3) {
            thread->quit();
        }
    });
    timer->start(1000);

    connect(thread, &QThread::finished, [] {
        qDebug() << "Thread finished";
    });
}

MyWidget::~MyWidget() {
    delete ui;
}

void MyWidget::closeEvent(QCloseEvent *event) {
    qDebug() << event->isAccepted();
}

void MyWidget::timerEvent(QTimerEvent *) {
    qDebug() << QThread::currentThread();
}

void MyWidget::initializeUi() {
    ui->setupUi(this);
}

void MyWidget::handleEvents() {

}

