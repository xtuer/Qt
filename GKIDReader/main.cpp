#include <QDebug>
#include <QTextCodec>
#include <QApplication>

#include "gui/MainWidget.h"
#include "gui/FramelessWindow.h"
#include "util/Util.h"
#include "util/UiUtil.h"
#include "util/ConfigUtil.h"
// #include "util/RunGuard.h"
#include "log/LogHandler.h"
#include "SingleApplication/singleapplication.h"

int main(int argc, char *argv[]) {
//    RunGuard guard("9F0FFF1A-77A0-4EF0-87F4-5494CA8181C7");

//    if (!guard.tryToRun()) {
//        return 0;
//    }

    SingleApplication app(argc, argv);
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    Singleton<LogHandler>::getInstance().installMessageHandler(); // 安装消息处理函数
    Util::registerTypes();

    UiUtil::loadQss();

    // qDebug() << "创建 Window";
    MainWidget *w = new MainWidget();

    FramelessWindow *window = new FramelessWindow(w);
    window->setTitleBarButtonsVisible(false, false, true);
    window->resize(400, 500);
    window->setResizable(false);
    window->show();

    QObject::connect(
        &app,
        &SingleApplication::instanceStarted,
        [=] {
            window->show();
            window->raise();
            window->activateWindow();
            window->raise();
            QApplication::setActiveWindow(window);
            window->raise();
        }
    );

    int ret = app.exec();
    Singleton<ConfigUtil>::getInstance().release();
    Singleton<LogHandler>::getInstance().release();

    return ret;
}
