#include "LogHandler.h"

#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <QDateTime>
#include <QMutexLocker>
#include <QtGlobal>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTimer>
#include <QTextStream>

/************************************************************************************************************
 *                                                                                                          *
 *                                               LogHandlerPrivate                                          *
 *                                                                                                          *
 ***********************************************************************************************************/
struct LogHandlerPrivate {
    LogHandlerPrivate();
    ~LogHandlerPrivate();

    // 打开日志文件 log.txt，如果日志文件不是当天创建的，则使用创建日期把其重命名为 yyyy-MM-dd.log，并重新创建一个 log.txt
    void openAndRenameLogFile();

    // 消息处理函数
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    QTimer renameLogFileTimer;  // 重命名日志文件使用的定时器
    QTimer flushLogFileTimer;   // 刷新输出到日志文件的定时器

    static QFile *logFile;      // 日志文件
    static QTextStream *logOut; // 输出日志的 QTextStream
    static QMutex logMutex;     // 同步使用的 mutex
};

QMutex LogHandlerPrivate::logMutex;
QFile* LogHandlerPrivate::logFile = NULL;
QTextStream* LogHandlerPrivate::logOut = NULL;

LogHandlerPrivate::LogHandlerPrivate() {
    openAndRenameLogFile();

    // 一个小时检查一次日志文件名
    renameLogFileTimer.setInterval(1000 * 60 * 60);
    renameLogFileTimer.start();
    QObject::connect(&renameLogFileTimer, &QTimer::timeout, [this] {
        QMutexLocker locker(&LogHandlerPrivate::logMutex);
        openAndRenameLogFile();
    });

    // 定时刷新日志输出到文件
    flushLogFileTimer.setInterval(1000);
    flushLogFileTimer.start();
    QObject::connect(&flushLogFileTimer, &QTimer::timeout, [this] {
        QMutexLocker locker(&LogHandlerPrivate::logMutex);
        if (NULL != logOut) {
            logOut->flush();
        }
    });
}

LogHandlerPrivate::~LogHandlerPrivate() {
    if (NULL != logFile) {
        logFile->flush();
        logFile->close();
        delete logOut;
        delete logFile;
    }
}

// 打开日志文件 log.txt，如果不是当天创建的，则使用创建日期把其重命名为 yyyy-MM-dd.log，并重新创建一个 log.txt
void LogHandlerPrivate::openAndRenameLogFile() {
    // 创建日志所在目录
    QDir logDir(".");
    if (!logDir.exists("log")) {
        logDir.mkdir("log");
    }

    // 程序启动时 file 为 NULL
    if (NULL == logFile) {
        logFile = new QFile("log/log.txt");
        logOut = (logFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) ?  new QTextStream(logFile) : NULL;

        // 可以检查日志文件超过 30 个，删除 30 天前的日志文件
    }

    QFileInfo info("log/log.txt");

    // 如果文件的创建日期不是运行时的日期，则重命名
    if (info.created().date() != QDate::currentDate()) {
        logFile->flush();
        logFile->close();
        delete logOut;
        delete logFile;

        QFile::rename("log/log.txt", info.created().date().toString("log/yyyy-MM-dd.log"));

        logFile = new QFile("log/log.txt");
        logOut = (logFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) ?  new QTextStream(logFile) : NULL;
    }
}

// 消息处理函数
void LogHandlerPrivate::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QMutexLocker locker(&LogHandlerPrivate::logMutex);
    QString level;

    switch (type) {
    case QtDebugMsg:
        level = "Debug";
        break;
    case QtInfoMsg:
        level = "Info";
        break;
    case QtWarningMsg:
        level = "Warn";
        break;
    case QtCriticalMsg:
        level = "Error";
        break;
    case QtFatalMsg:
        level = "Fatal";
        break;
    default:;
    }

    // 输出到标准错误输出
    QByteArray localMsg = msg.toUtf8();
    fprintf(stderr, "%s\n", localMsg.constData());

    if (NULL == LogHandlerPrivate::logOut) {
        return;
    }

    // 输出到日志文件, 格式: 时间 - [Level] (文件名:行数, 函数): 消息
    QString fileName = context.file;
    int index = fileName.lastIndexOf('/');
    fileName = fileName.mid(index + 1);

    (*LogHandlerPrivate::logOut) << QString("%1 - [%2] (%3:%4, %5): %6\n")
                                    .arg(QTime::currentTime().toString("hh:mm:ss")).arg(level)
                                    .arg(fileName).arg(context.line).arg(context.function).arg(msg);
}

/************************************************************************************************************
 *                                                                                                          *
 *                                               LogHandler                                                 *
 *                                                                                                          *
 ***********************************************************************************************************/
LogHandler::LogHandler() {
    d = new LogHandlerPrivate();
}

LogHandler::~LogHandler() {

}

void LogHandler::installMessageHandler() {
    qInstallMessageHandler(LogHandlerPrivate::messageHandler);
}

void LogHandler::release() {
    delete d;
}
