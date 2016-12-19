#include "BroadcastReceiver.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    BroadcastReceiver receiver;
    return a.exec();
}
