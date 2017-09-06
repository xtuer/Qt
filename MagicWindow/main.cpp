#include "MagicWindow.h"
#include <QApplication>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QTextCodec>

int sum(int a, int b) {
    return a + b;
}

typedef int (*Fun)(int, int);

int main(int argc, char *argv[]) {
//    QApplication a(argc, argv);

    Fun fun = sum;
    qDebug() << fun(3, 4);

    return 0;
}
