#include "ClipWidget.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    ClipWidget w;
    w.show();
    return a.exec();
}
