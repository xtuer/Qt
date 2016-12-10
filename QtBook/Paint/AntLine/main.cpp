#include "AntLineWidget.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    AntLineWidget w;
    w.show();

    return a.exec();
}
