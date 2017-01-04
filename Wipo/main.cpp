#include "gui/MainWidget.h"
#include "util/UiUtil.h"

#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    UiUtil::loadQss();

    MainWidget w;
    w.move(QApplication::desktop()->availableGeometry().center() - w.rect().center()); // 居中
    w.show();

    return a.exec();
}
