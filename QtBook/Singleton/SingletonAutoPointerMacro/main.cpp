#include "ConfigUtil.h"
#include <QDebug>

int main(int argc, char *argv[]) {
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    qDebug() << ConfigUtil::getInstance().getDatabaseName();
    qDebug() << ConfigUtil::getInstance().getDatabaseName();

    return 0;
}
