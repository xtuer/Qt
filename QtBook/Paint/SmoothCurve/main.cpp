#include "SmoothCurveWidget.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    SmoothCurveWidget w;
    w.show();

    return a.exec();
}
