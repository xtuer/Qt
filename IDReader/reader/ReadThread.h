#ifndef READTHREAD_H
#define READTHREAD_H

#include <QThread>

class CardReader;
class Person;

class ReadThread : public QThread {
    Q_OBJECT

public:
    ReadThread(QObject *parent = 0);
    ~ReadThread();

    void stop();

signals:
    void personReady(const Person &person);
    void info(int infoCode, const QString &infoString);

protected:
    void run() Q_DECL_OVERRIDE;

private:
    void handleInfo(int infoCode, const QString &infoString);

    CardReader *reader;
    bool stopped;
};

#endif // READTHREAD_H
