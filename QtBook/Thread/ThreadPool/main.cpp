// 文件名: main.cpp

#include <QApplication>
#include <QThreadPool>
#include "Task.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    for (int i = 1; i <= 100; ++i) {
        Task *task = new Task(i); // 创建任务
        QThreadPool::globalInstance()->start(task); // 提交任务给线程池，在线程池中执行
    }

    return a.exec();
}
