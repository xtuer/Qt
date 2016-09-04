#include "widget.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Widget w;
    w.show();

    return app.exec();
}
