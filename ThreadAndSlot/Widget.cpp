#include "Widget.h"
#include "XThread.h"
#include <QDebug>
#include <QMetaObject>
#include <QLabel>
#include <QHBoxLayout>

Widget::Widget(QWidget *parent) : QWidget(parent) {
    // 界面布局
    timeLabel = new QLabel("");
    QHBoxLayout *hl = new QHBoxLayout();
    hl->addWidget(new QLabel("线程里的信号触发修改:"));
    hl->addWidget(timeLabel);
    this->setLayout(hl);

    // 创建启动线程
    thread = new XThread();
    thread->start();

    // 事件处理 1
    connect(thread, &XThread::currentTime, [this](const QString &time) {
        qDebug() << "connect:  " << QThread::currentThread(); // 当前环境的上下文属于线程 XThread
        this->showTime(time); // Error: 有时候没问题，有时候会有警告，有的时候程序直接退出，所以不要这么做，相当于在 XThread 中直接调用
        QMetaObject::invokeMethod(this, "showTime", Q_ARG(QString, time)); // OK: 一个线程调中用另外一个线程中函数的正确姿势
    });

    // 事件处理 2
    connect(thread, &XThread::currentTime, this, &Widget::showTime); // OK: 使用信号槽
}

Widget::~Widget() {
}

void Widget::showTime(const QString &time) {
    qDebug() << "showTime: " << QThread::currentThread(); // 使用 invokeMethod() 调用时属于 Ui 线程
    this->timeLabel->setText(time);
}
