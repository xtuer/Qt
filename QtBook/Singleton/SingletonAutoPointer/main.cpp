#include "ConfigUtil.h"
#include <QDebug>

int main(int argc, char *argv[]) {
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    qDebug() << Config::getInstance().getDatabaseName();
    qDebug() << Config::getInstance().getDatabaseName();

    return 0;
}
