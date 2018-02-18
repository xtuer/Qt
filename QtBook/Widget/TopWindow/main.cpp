#include "gui/TopWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget *centralWidget = new QWidget();
    centralWidget->setStyleSheet("background: #2d8cf0;");

    TopWindow window(centralWidget);
    window.setTitle("自定义标题栏无边框阴影窗口");
    window.show();

    return app.exec();
}
