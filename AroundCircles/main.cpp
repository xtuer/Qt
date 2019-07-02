#include <QApplication>
#include <QDebug>
#include "ArrangeDevicesWidget.h"
#include "AroundCirclesGraphicsView.h"

#include <QGraphicsView>
#include <QGraphicsScene>

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    ArrangeDevicesWidget w;
    w.show();

//    AroundCirclesGraphicsView w;
//    w.resize(500, 500);
//    w.show();

    return app.exec();
}
