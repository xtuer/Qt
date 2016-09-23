#ifndef LOGHANDLER_H
#define LOGHANDLER_H

#include "Singleton.h"

class QString;
enum QtMsgType;
class QMessageLogContext;
struct LogHandlerPrivate;

class LogHandler {
    SINGLETON(LogHandler)
public:
    void release(); // 释放资源
    void installMessageHandler(); // 给 Qt 安装消息处理函数

private:
    LogHandlerPrivate *d;
};

#endif // LOGHANDLER_H
