#include "CalloutChartWidget.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    CalloutChartWidget w;
    w.show();

    return a.exec();
}
