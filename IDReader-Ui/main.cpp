#include "gui/MainWidget.h"
#include "gui/FramelessWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainWidget *w = new MainWidget();
    FramelessWindow *window = new FramelessWindow(w);
    window->setTitleBarButtonsVisible(false, false, true);
    window->resize(540, 500);
    window->setResizable(false);
    window->show();

    int ret = a.exec();
    return ret;
}
