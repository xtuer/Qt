#include "Thread.h"
#include "db/DBUtil.h"

Thread::Thread() {

}

void Thread::run() {
    qDebug() << DBUtil::selectInt("select id from user where username='Alice'");
    QThread::sleep(1);
    qDebug() << DBUtil::selectInt("select id from user where username='Bob'");
}
