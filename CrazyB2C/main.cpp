#include "gui/CentralWidget.h"
#include "util/UiUtil.h"
#include "db/ConnectionPool.h"
#include "test/Test.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
    // [[0]] 启动程序，加载样式 QSS 文件，翻译文件，登陆等
    QApplication a(argc, argv);
    UiUtil::loadQss();

    // [[1]] 测试
    Test::test();

    // [[2]] 创建并显示窗口
    CentralWidget w;
    w.show();

    // [[3]] 进入 Qt Gui 事件循环
    int state = a.exec();

    // [[4]] 清理释放资源，例如销毁连接池
    ConnectionPoolInstance.destroy();

    // [[5]] 结束程序
    return state;
}
