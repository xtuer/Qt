#include <QApplication>
#include <QDebug>
#include "ArrangeDevicesWidget.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    ArrangeDevicesWidget w;
    w.show();

    return app.exec();
}
