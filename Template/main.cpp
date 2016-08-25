#include "gui/MainWindow.h"
#include "gui/CentralWidget.h"

#include "util/UiUtil.h"
#include "util/CommonUtil.h"

#include "test/Test.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
//    // [[0]] 设置 Debug 输出的格式
//    CommonUtil::registerMessagePattern();

//    Test::test();

    QApplication a(argc, argv);
    UiUtil::loadQss();

    // [[1]] 创建并显示窗口
    CentralWidget w;
    w.show();

    // [[2]] 进入 Qt Gui 事件循环
    int ret = a.exec();

//    // [[3]] 清理释放资源
    CommonUtil::releaseSingletonResource();
//    return ret;
}
