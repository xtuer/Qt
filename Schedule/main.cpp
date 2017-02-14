#include "gui/MainWidget.h"
#include "gui/NoFocusRectStyle.h"
#include "db/DBUtil.h"
#include "util/UiUtil.h"
#include "test/Test.h"
#include "MagicWindow/MagicWindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    NoFocusRectStyle *style = new NoFocusRectStyle(app.style());
    app.setStyle(style); // Ownership of the style object is transferred to QApplication
    UiUtil::loadQss();

    MainWidget *w = new MainWidget();
    // 很重要，因为我们需要在 qApp 的 aboutToQuit() 信号对应的槽函数里释放资源，所以释放资源前主窗口需要先被析构
    //    w->setAttribute(Qt::WA_DeleteOnClose);
    //    w->show();

    MagicWindow *window = new MagicWindow(w);
    window->setTitle("排课精灵");
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();

    // Test::test();

    return app.exec();
}
