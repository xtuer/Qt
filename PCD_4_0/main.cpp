#include "CentralWidget.h"
#include "UiUtil.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    UiUtil::loadQss();

    CentralWidget w;
    UiUtil::installLoadQssShortcut(&w);

    w.show();
    return a.exec();
}
