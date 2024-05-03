#include "User.h"

User::User(const QString &id, const QString &username, const QString &password, const Permission &permission)
    : id(id), username(username), password(password), permission(permission) {
}

// id 大于 0 时为有效用户 (数据库里自增长 ID 从 1 开始)
bool User::isValid() const {
    return !id.isEmpty();
}

QDebug operator<<(QDebug stream, const User &user) {
    stream.noquote() << QString("ID: %1, Username: %2, Password: %3, OrgId: %4")
                        .arg(user.id).arg(user.username).arg(user.password).arg(user.orgId)
                     << QString(", Permission: ") << user.permission.getPermissions()
                     << QString(", DeviceEnabled: ") << user.deviceEnabled
                     << QString(", Devices: ") << user.devices
                     << QString("\nAuthToken: %1").arg(user.authToken);
    return stream;
}
