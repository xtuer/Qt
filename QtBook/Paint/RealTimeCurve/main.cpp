#include "RealTimeCurveWidget.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    RealTimeWidget w;
    w.show();

    return a.exec();
}
