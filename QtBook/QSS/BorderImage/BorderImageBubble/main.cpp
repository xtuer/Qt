#include "BubbleWidget.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    BubbleWidget w;
    w.show();
    return a.exec();
}
