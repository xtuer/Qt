#include "ConfigUtil.h"
#include "Singleton.h"
#include <QDebug>

int main(int argc, char *argv[]) {
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    qDebug() << Singleton<ConfigUtil>::getInstance().getDatabaseName();
    qDebug() << Singleton<ConfigUtil>::getInstance().getDatabaseName();

    return 0;
}
