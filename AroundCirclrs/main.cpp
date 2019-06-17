#include <QApplication>
#include <QDebug>
#include "ArrangeDevicesWidget.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    ArrangeDevicesWidget w;
//    w.setColor(1, Qt::red);
//    w.setColor(10, Qt::yellow);
//    w.setColor(21, Qt::green);
    w.show();

    return app.exec();
}
