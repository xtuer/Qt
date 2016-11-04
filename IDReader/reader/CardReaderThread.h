#ifndef CARDREADERTHREAD_H
#define CARDREADERTHREAD_H

#include <QThread>

class CardReader;
class Person;

class CardReaderThread : public QThread {
    Q_OBJECT

public:
    CardReaderThread(QObject *parent = 0);
    ~CardReaderThread();

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

#endif // CARDREADERTHREAD_H
