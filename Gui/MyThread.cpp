#include "MyThread.h"

void MyThread::run() {
    while (true) {
        emit shout();
        QThread::msleep(1000);
    }
}
