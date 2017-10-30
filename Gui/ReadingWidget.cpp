#include "ReadingWidget.h"
#include "ui_ReadingWidget.h"
#include "ReadingThread.h"
#include <QDebug>

ReadingWidget::ReadingWidget(QWidget *parent) : QWidget(parent), ui(new Ui::ReadingWidget) {
    ui->setupUi(this);

    // 创建线程对象
    readingThread = new ReadingThread(ui->textEdit, this);

    // 点击 "开始读取" 按钮启动线程
    connect(ui->startButton, &QPushButton::clicked, [this] {
        if (!readingThread->isRunning()) {
            // 线程没有运行时才启动，调用 start() 启动线程
            readingThread->start();
        } else {
            qDebug() << "线程正在运行";
        }
    });

    // 点击 "结束读取" 按钮结束线程
    connect(ui->stopButton, &QPushButton::clicked, [this] {
        readingThread->stop(); // 提示结束线程执行
        readingThread->wait(); // 等待线程真正的结束执行
    });

    // 线程结束时的信号槽
    connect(readingThread, &QThread::finished, [] {
        qDebug() << "线程结束运行";
    });
}

ReadingWidget::~ReadingWidget() {
    delete ui;
}
