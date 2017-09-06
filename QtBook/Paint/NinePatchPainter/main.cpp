#include "Widget.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Widget w;
    w.setWindowTitle("九宫格绘图");
    w.resize(660, 240);
    w.show();

    return a.exec();
}
