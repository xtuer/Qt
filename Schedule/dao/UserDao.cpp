#include "UserDao.h"
#include "bean/User.h"
#include "db/DBUtil.h"
#include "db/Sqls.h"

const char * const SQL_NAMESPACE_USER = "User";

User UserDao::findByUserId(int id) {
    QString sql = Singleton<Sqls>::getInstance().getSql(SQL_NAMESPACE_USER, "findByUserId").arg(id);
    return DBUtil::selectBean(mapToUser, sql);
}

QList<User> UserDao::findtAll() {
    QString sql = Singleton<Sqls>::getInstance().getSql(SQL_NAMESPACE_USER, "findAll");
    return DBUtil::selectBeans(mapToUser, sql);
}

int UserDao::insert(const User& user) {
    QString sql = Singleton<Sqls>::getInstance().getSql(SQL_NAMESPACE_USER, "insert");

    QVariantMap params;
    params["username"] = user.username;
    params["password"] = user.password;
    params["email"]    = user.email;
    params["mobile"]   = user.mobile;

    return DBUtil::insert(sql, params);
}

bool UserDao::update(const User& user) {
    QString sql = Singleton<Sqls>::getInstance().getSql(SQL_NAMESPACE_USER, "update");

    QVariantMap params;
    params["id"]       = user.id;
    params["username"] = user.username;
    params["password"] = user.password;
    params["email"]    = user.email;
    params["mobile"]   = user.mobile;

    return DBUtil::update(sql, params);
}

User UserDao::mapToUser(const QVariantMap &rowMap) {
    User user;

    user.id = rowMap.value("id", -1).toInt();
    user.username = rowMap.value("username").toString();
    user.password = rowMap.value("password").toString();
    user.email    = rowMap.value("email").toString();
    user.mobile   = rowMap.value("mobile").toString();

    return user;
}
