#include "MouseSelectionWidget.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MouseSelectionWidget w;
    w.show();

    return a.exec();
}
