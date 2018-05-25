#include "gui/AiSignWidget.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    AiSignWidget w;
    w.show();

    return a.exec();
}
