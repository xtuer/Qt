#ifndef THREAD_H
#define THREAD_H

#include <QThread>

class Thread : public QThread {
    Q_OBJECT
public:
    Thread();

protected:
    void run() override;

signals:
    void beat();
};

#endif // THREAD_H
