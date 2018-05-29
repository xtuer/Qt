#include "gui/AiSignWidget.h"
#include "gui/TopWindow.h"
#include "util/Util.h"
#include "util/UiUtil.h"
#include "util/Config.h"
#include "util/LogHandler.h"

#include <QApplication>
#include <QTextCodec>
#include <QDebug>

static void initialize(); // 程序启动时进行初始化
static void finalize();   // 程序结束时清理工作

int main(int argc, char *argv[]) {
    // 启用 Retina 高分辨率
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    ::initialize();

    AiSignWidget *aiSignWidget = new AiSignWidget();
    TopWindow *window = new TopWindow(aiSignWidget, {0, 0, 0, 0}, {0, 0, 0, 0});
    window->setTitle("");
    // window->resize(1180, 700);
    // window->setResizable(false);
    // window->setFixedSize(880, 700);
    window->setTitleBarButtonsVisible(false, false, true);
    UiUtil::installLoadQssShortcut(window);
    // UiUtil::centerWindow(window);
    // window->show();
    window->showMaximized();

    // [3] 进入 Qt 事件队列
    int code = app.exec();
    delete window;
    ::finalize();
    return code;
}

/**
 * 程序启动时进行初始化
 */
static void initialize() {
    // 使用 UTF-8 的运行时环境
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    // 安装日志处理工具
    LogHandlerInstance.installMessageHandler();

    // 注册自定义类型，跨线程信号槽使用
    Util::registerTypes();

    // 设置界面样式
    UiUtil::loadQss();

    // 加载字体库
    UiUtil::loadFonts();
    qDebug() << QString("你好");
}

/**
 * 程序结束时清理工作
 * 程序结束时释放回收资源，例如释放日志资源，释放数据库连接池资源等
 */
static void finalize() {
    ConfigInstance.destroy();
    LogHandlerInstance.uninstallMessageHandler(); // 程序结束时释放 LogHandler 的资源，例如刷新并关闭日志文件
}
