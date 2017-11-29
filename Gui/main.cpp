#include <QApplication>
#include <QDebug>
#include "Widget.h"

int main(int argc, char *argv[]) {
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    qDebug() << LLONG_MAX;

    return 0;
}
