#include "PaintButtonWidget.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    PaintButtonWidget w;
    w.show();

    return a.exec();
}
