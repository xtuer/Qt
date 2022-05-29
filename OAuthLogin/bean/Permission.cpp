#include <QObject>
#include "Permission.h"

const QMap<PermissionFlag, QString> Permission::PERMISSIONS {
    { PermissionFlag::PROGRAM_RECORD,  QObject::tr("编程记录器") },
    { PermissionFlag::READ_RECORD,     QObject::tr("读取记录器") },
    { PermissionFlag::MODIFY_SECURITY, QObject::tr("更改安全选项") },
    { PermissionFlag::MODIFY_SETTINGS, QObject::tr("更改记录器名") },
    { PermissionFlag::AUDIT_TRAIL,     QObject::tr("显示审计追踪") },
    { PermissionFlag::USERMANG,        QObject::tr("用户管理") },
    { PermissionFlag::UNKOWN,          QObject::tr("保留权限") },
    { PermissionFlag::REPORT,          QObject::tr("生成报告") },
    { PermissionFlag::OPEN_FILE,       QObject::tr("导入数据") },
};

Permission::Permission(PermissionFlags permissions) :permissions(permissions) {

}

// 获取权限
PermissionFlags Permission::getPermissions() const {
    return permissions;
}

// 获取权限的整形值，存储到数据库，文件里时有用
int Permission::getPermissionsValue() const {
    return PermissionFlags::Int(permissions);
}

// 检测是否有权限
bool Permission::hasPermission(PermissionFlag permission) const {
    return permissions.testFlag(permission);
}

// 增加权限
void Permission::addPermission(PermissionFlag permission) {
    permissions |= permission;
}

// 删除权限
void Permission::removePermission(PermissionFlag permission) {
    permissions &= (~permission);
}
