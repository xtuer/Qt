#include <QApplication>
#include "Widget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Widget window;
    window.resize(500, 400);
    window.setStyleSheet(".Scatter { background: #2b85e4; border-radius: 10px; min-width: 20px; max-width: 20px; min-height: 20px; max-height: 20px; }");
    window.show();

    return a.exec();
}
