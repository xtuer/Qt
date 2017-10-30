#ifndef READINGTHREAD_H
#define READINGTHREAD_H

#include <QThread>

class QTextEdit;

class ReadingThread : public QThread {
public:
    ReadingThread(QTextEdit *textEdit, QObject *parent = NULL);
    void stop(); // 结束线程

protected:
    void run() Q_DECL_OVERRIDE;

private:
    bool stopped; // 线程结束的标志
    QTextEdit *textEdit;
};

#endif // READINGTHREAD_H
