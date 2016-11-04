#include "gui/MainWidget.h"
#include "gui/FramelessWindow.h"
#include "util/Util.h"
#include "util/ConfigUtil.h"
#include "log/LogHandler.h"
#include <QApplication>
#include <QDebug>
#include <QTextCodec>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    Singleton<LogHandler>::getInstance().installMessageHandler(); // 安装消息处理函数
    Util::registerTypes();

    // qDebug() << "创建 Window";
    MainWidget *w = new MainWidget();

    FramelessWindow *window = new FramelessWindow(w);
    window->setTitleBarButtonsVisible(false, false, true);
    window->resize(540, 500);
    window->setResizable(false);
    window->show();

    int ret = a.exec();
    Singleton<ConfigUtil>::getInstance().release();
    Singleton<LogHandler>::getInstance().release();

    return ret;
}
