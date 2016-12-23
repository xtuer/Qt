#include "ConfigUtil.h"
#include <QDebug>

int main(int argc, char *argv[]) {
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    qDebug() << Singleton<Config>::getInstance().getDatabaseName();
    qDebug() << Singleton<Config>::getInstance().getDatabaseName();
    qDebug() << ConfigUtilInstance.getDatabaseName();

    return 0;
}
