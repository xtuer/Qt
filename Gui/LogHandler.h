#ifndef LOGHANDLER_H
#define LOGHANDLER_H

#include <QMutex>
#include <QFile>
#include <QTimer>

class LogHandler {
public:
    static void initialize();
    static void destroy();

    static QFile *file;
    static QMutex mutex;
    static QTimer timer;
    static QDate  today;
};

#endif // LOGHANDLER_H
