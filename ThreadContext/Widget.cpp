#include "Widget.h"
#include "Thread.h"
#include <QDebug>
#include <QMetaObject>

// [+] 表示属于 Ui 线程
// [-] 表示属于 Thread 线程
Widget::Widget(QWidget *parent) : QWidget(parent) {
    qDebug() << "1-Widget::Widget(): " << QThread::currentThread(); // [+] 属于 Ui 线程
    thread = new Thread();                                          // 创建线程对象
    qDebug() << "3-Widget.thread: " << thread->thread();            // [+] 属于 Ui 线程: thread 属于创建它的线程
    this->directCall();                                             // [+] 属于 Ui 线程: 被调用的函数中

    // 事件处理 1
    connect(thread, &Thread::beat, [this]() {
        qDebug() << "Lambda-1: " << QThread::currentThread(); // [-] 属于 Thread 的线程: 当前函数中
        this->directCall();                                   // [-] 属于 Thread 的线程: 被调用的函数中
        QMetaObject::invokeMethod(this, "invokeCall");        // [+] 属于 Ui 线程: 被调用的函数中
    });

    // 事件处理 2
    // 第 3 个参数 context this 指定了 Lambda 里的线程为 Ui 线程
    connect(thread, &Thread::beat, this, [this]() {
        qDebug() << "Lambda-2: " << QThread::currentThread(); // [+] 属于 Ui 线程: 当前函数中
        this->directCall();                                   // [+] 属于 Ui 线程: 被调用的函数中
        QMetaObject::invokeMethod(this, "invokeCall");        // [+] 属于 Ui 线程: 被调用的函数中
    });

    // 事件处理 3
    connect(thread, &Thread::beat, this, &Widget::slotCall); // [+] 属于 Ui 线程: 被调用的函数中

    // 事件处理 4
    connect(thread, SIGNAL(beat()), this, SLOT(slotCall())); // [+] 属于 Ui 线程: 被调用的函数中

    thread->start();
}

Widget::~Widget() {}

void Widget::directCall() {
    // [+] 属于 Ui 线程
    // [-] 属于 Thread 线程
    // 直接调用的函数属于调用它时代码所在的线程，不管它是谁的函数
    qDebug() << "directCall: " << QThread::currentThread();
}

void Widget::invokeCall() {
    // [+] 属于 Ui 线程
    // invokeMethod() 调用的函数属于它的对象所在的线程，不管它在哪个线程中
    qDebug() << "invokeCall: " << QThread::currentThread();
}

void Widget::slotCall() {
    // [+] 属于 Ui 线程
    // 传统 signal slot 调用的函数属于 receiver 说在的线程
    qDebug() << "slotCall: " << QThread::currentThread();
}
