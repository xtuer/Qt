#include "widget.h"

#include <QDebug>
#include <QApplication>
#include <QTimer>
#include <QDateTime>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Widget w;
    w.show();

    return app.exec();
}
