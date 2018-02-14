#include "gui/TopWindow.h"
#include "gui/DemoWidget.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    // 启用 Retina 高分辨率
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    DemoWidget *c = new DemoWidget();
//    TopWindow w(c);
    TopWindow w(c);//, {4, 4, 4, 4}, {8, 8, 8, 8}, ":/image/colorful-border.png");
//    w.setResizable(false);
//    w.setTitleBarButtonsVisible(false, false, true);
//    w.setTitleBarVisible(false);
    w.resize(600, 500);
    w.show();

    TopWindow::message("Android 9.0 开发代号定为 Pistachio Ice Cream\n"
                       "码云推荐 | 点对点服务治理框架 Antenna\n");

    return app.exec();
}
