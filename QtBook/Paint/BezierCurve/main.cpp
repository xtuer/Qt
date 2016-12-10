#include "BezierCurveWidget.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    BezierCurveWidget w;
    w.show();

    return a.exec();
}
