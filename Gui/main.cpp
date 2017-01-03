#include <QApplication>
#include <QDebug>
#include <QList>
#include "Widget.h"
#include <QPushButton>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Widget w;
    w.show();

    return app.exec();
}
