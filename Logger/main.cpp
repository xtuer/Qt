#include "LogHandler.h"

#include <QApplication>
#include <QDebug>
#include <QTime>
#include <QPushButton>
#include <QTextCodec>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // [[1]] 安装消息处理函数
    Singleton<LogHandler>::getInstance().installMessageHandler();

    // [[2]] 输出测试，查看是否写入到文件
    qDebug() << "Hello";
    qDebug() << "当前时间是: " << QTime::currentTime().toString("hh:mm:ss");
    qInfo() << QString("God bless you!");

    QPushButton *button = new QPushButton("退出");
    button->show();
    QObject::connect(button, &QPushButton::clicked, [&app] {
        qDebug() << "退出";
        app.quit();
    });

    // [[3]] 删除自定义消息处理，然后启用
    Singleton<LogHandler>::getInstance().release();
    qDebug() << "........"; // 不写入日志
    Singleton<LogHandler>::getInstance().installMessageHandler();

    int ret = app.exec(); // 事件循环结束

    // [[4]] 程序结束时释放 LogHandler 的资源，例如刷新并关闭日志文件
    Singleton<LogHandler>::getInstance().release();

    return ret;
}
