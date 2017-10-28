#include "RealTimeCurveQChartWidget.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    RealTimeCurveQChartWidget w;
    w.resize(700, 400);
    w.show();

    return a.exec();
}
