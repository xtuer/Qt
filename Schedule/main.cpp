#include "gui/MainWidget.h"
#include "db/DBUtil.h"
#include "db/ConnectionPool.h"
#include "util/UiUtil.h"
#include "util/ConfigUtil.h"
#include "test/Test.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    UiUtil::loadQss();
    MainWidget *w = new MainWidget();
    w->show();

//    Test::test();

    int ret = a.exec();

    // 释放资源
    delete w;
    Singleton<ConnectionPool>::getInstance().release();
    Singleton<ConfigUtil>::getInstance().release();

    return ret;
}
