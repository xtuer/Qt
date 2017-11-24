#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include "Widget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    qDebug() << QDateTime::currentSecsSinceEpoch();

    return app.exec();
}
