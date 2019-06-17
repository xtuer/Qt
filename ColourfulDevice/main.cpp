#include "ColourfulDeviceWidget.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    ColourfulDeviceWidget w;
    w.show();

    return a.exec();
}
