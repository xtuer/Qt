#ifndef XTHREAD_H
#define XTHREAD_H

#include <QThread>

class XThread : public QThread {
    Q_OBJECT
public:
    XThread();

protected:
    void run() Q_DECL_OVERRIDE;

signals:
    void currentTime(const QString &time);
};

#endif // XTHREAD_H
