#include <QApplication>
#include "MainWidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWidget w;
    w.show();

//    BomUtils::insertBom("/Users/Biao/Desktop/foo.txt");
//    BomUtils::removeBom("/Users/Biao/Desktop/foo.txt");

    return app.exec();
}
