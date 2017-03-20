#include <QApplication>
#include "OSUi.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    OSUi w;
    w.show();
    return a.exec();
}
