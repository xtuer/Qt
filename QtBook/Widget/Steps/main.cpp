#include "StepWidget.h"
#include "UiUtil.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    UiUtil::loadQss();

    StepWidget w;
    w.show();

    return a.exec();
}
