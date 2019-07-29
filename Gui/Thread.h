#ifndef THREAD_H
#define THREAD_H
#include <QThread>

class Thread : public QThread {
public:
    Thread();

protected:
//    void run() override;
    void timerEvent(QTimerEvent *event) override;
};

#endif // THREAD_H
