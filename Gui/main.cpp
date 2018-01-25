#include <QApplication>
#include <QDebug>
#include "ImageWidget.h"
#include <QStringList>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ImageWidget w;
    w.show();

    return app.exec();
}
