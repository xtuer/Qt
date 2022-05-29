#ifndef USER_H
#define USER_H

#include "Permission.h"
#include <QString>
#include <QDebug>
#include <QDate>

class User {
public:
    /**
     * 创建用户
     *
     * @param id 默认为空，为空时表示用户不是有效用户
     * @param username 账号
     * @param password 密码
     * @param permission 权限
     */
    User(const QString &id = QString(),
         const QString &username = QString(),
         const QString &password = QString(),
         const Permission &permission = Permission());

    bool isValid() const; // id 大于 0 时为有效用户 (数据库里自增长 ID 从 1 开始)

    friend QDebug operator<<(QDebug stream, const User &user);

    QString id;        // 用户 ID
    QString username;  // 账号
    QString password;  // 密码
    QString authToken; // 登录 token
    QString orgId;     // 用户所属机构 ID
    QString creator;   // 操作员
    QDate   expiredAt; // 有效期
    bool    enabled = true; // 是否可用
    Permission permission;   // 权限
};

#endif // USER_H
