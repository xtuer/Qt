#include "gui/UsersWidget.h"
#include "gui/CreateUserWidget.h"
#include "bean/Permission.h"
#include "db/ConnectionPool.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    UsersWidget w;
    w.show();

    int ret = a.exec();
    ConnectionPoolInstance.destroy();

    return ret;
}
