#include "MulticastReciver.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MulticastReciver receiver;
    return a.exec();
}
