#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>

class MyThread : public QThread {
    Q_OBJECT

signals:
    void shout();

protected:
    void run() Q_DECL_OVERRIDE;
};

#endif // MYTHREAD_H
