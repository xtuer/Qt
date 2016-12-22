#include "ConnectionPool.h"
#include "TestConnectionPoolWithMultiThread.h"
#include <QApplication>
#include <QPushButton>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QPushButton *button = new QPushButton("Access Database");
    button->show();

    QObject::connect(button, &QPushButton::clicked, []() {
        for (int i = 0; i < 100; ++i) {
            TestConnectionPoolWithMultiThread *thread = new TestConnectionPoolWithMultiThread(i);
            thread->start();
             QThread::msleep(50);
        }
    });

    int ret = a.exec();
    ConnectionPool::destroy(); // 程序结束时销毁数据库连接池
    return ret;
}
