#include <QApplication>
#include "ScatterWidget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    ScatterWidget window;
    window.resize(770, 450);
    window.setStyleSheet(".Scatter { background: #2b85e4; border-radius: 10px; min-width: 20px; max-width: 20px; min-height: 20px; max-height: 20px; }");
    window.show();

    return a.exec();
}
