#ifndef TESTCONNECTIONPOOLWITHMULTITHREAD_H
#define TESTCONNECTIONPOOLWITHMULTITHREAD_H

#include <QThread>

class TestConnectionPoolWithMultiThread : public QThread {
public:
    TestConnectionPoolWithMultiThread(int sn);

protected:
    void run();
    int sn;
};

#endif // TESTCONNECTIONPOOLWITHMULTITHREAD_H
