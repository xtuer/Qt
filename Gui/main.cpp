#include <QApplication>
#include <QTreeView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QMap>
#include <QList>
#include <QAction>
#include <QDebug>
#include "Permission.h"
#include "AddUserWidget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    AddUserWidget w;
    w.show();

    return a.exec();
//    Permission p(PermissionFlags(1223));
//    Permission p2(PermissionFlag::ADMIN | PermissionFlag::GUEST);
//    p.addPermission(PermissionFlag::ADMIN);
//    p.addPermission(PermissionFlag::GUEST);

//    qDebug() << p.hasPermission(PermissionFlag::ADMIN);
//    qDebug() << QString("%1").arg(p.getPermissions(), 0, 2);
//    qDebug() << p.getPermissionsValue();

//    foreach (PermissionFlag p, Permission::PERMISSIONS.keys()) {
//        qDebug() << Permission::PERMISSIONS.value(p);
//    }

//    PermissionFlags fs(7);
//    qDebug() << fs << PermissionFlags::Int(fs);

//    return 0;
}
