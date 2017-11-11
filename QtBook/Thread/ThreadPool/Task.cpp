// 文件名: Task.cpp
#include "Task.h"
#include <QDebug>
#include <QThread>
#include <QDateTime>

Task::Task(int id) : id(id) {

}

Task::~Task() {
    qDebug().noquote() << QString("~Task() with ID %1").arg(id); // 方便查看对象是否被 delete
}

void Task::run() {
    qDebug().noquote() << QString("Start thread %1 at %2").arg(id).arg(QDateTime::currentDateTime().toString("mm:ss.z"));
    QThread::msleep(500 + qrand() % 2000); // 每个 run() 函数随机执行 [55, 2500] 毫秒，模拟耗时任务
    qDebug().noquote() << QString("End   thread %1 at %2").arg(id).arg(QDateTime::currentDateTime().toString("mm:ss.z"));
}
