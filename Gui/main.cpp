#include <QApplication>
#include "Carousel.h"
#include <QDebug>
#include <QObject>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Carousel w;
    w.show();
    return app.exec();
}
