#include "CentralWidget.h"
#include "UiUtil.h"

#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    UiUtil::loadQss();

    CentralWidget w;
    UiUtil::installLoadQssShortcut(&w);

    QPixmap pixmap("PCD_4.0-resources/img/01-startup-cn.png");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.finish(&w);

    w.show();

    return a.exec();
}
