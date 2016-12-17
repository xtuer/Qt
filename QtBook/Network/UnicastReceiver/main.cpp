#include <QCoreApplication>
#include "UnicastReceiver.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    UnicastReceiver receiver;
    return a.exec();
}
