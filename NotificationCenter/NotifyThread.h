#ifndef NOTIFYTHREAD_H
#define NOTIFYTHREAD_H
#include <QThread>

class NotifyThread : public QThread {
    Q_OBJECT
public:
    NotifyThread();

protected:
    void run() Q_DECL_OVERRIDE;
};

#endif // NOTIFYTHREAD_H
