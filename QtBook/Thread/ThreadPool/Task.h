// 文件名: Task.h
#ifndef TASK_H
#define TASK_H

#include <QRunnable>

class Task : public QRunnable {
public:
    Task(int id);
    ~Task();

    void run() Q_DECL_OVERRIDE;

private:
    int id; // 线程的 ID
};

#endif // TASK_H
