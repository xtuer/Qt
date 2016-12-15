#include "ConfigUtil.h"
#include <QDebug>

int main(int argc, char *argv[]) {
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    qDebug() << ConfigUtil::getInstance().getDatabaseName();
    qDebug() << ConfigUtil::getInstance().getDatabaseName();

    ConfigUtil::release(); // 程序结束时需要手动析构 ConfigUtil 的对象

    return 0;
}
