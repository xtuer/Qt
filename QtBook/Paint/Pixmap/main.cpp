#include "PixmapWidget.h"
#include <QApplication>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    PixmapWidget w;
    w.show();

    return a.exec();
}
