#include "gui/TopWindow.h"
#include "gui/DemoWidget.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    // 启用 Retina 高分辨率
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    DemoWidget *demoWidget = new DemoWidget();
    TopWindow window(demoWidget);//, {4, 4, 4, 4}, {8, 8, 8, 8}, ":/image/top-window/colorful-border.png");
    window.resize(600, 500);
    window.show();

    return app.exec();
}
