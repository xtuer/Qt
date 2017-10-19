#include <QApplication>
#include "FittingCurveWidget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    FittingCurveWidget widget;
    widget.resize(700, 400);
    widget.show();

    return a.exec();
}
