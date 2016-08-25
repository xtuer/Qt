#include "UserDao.h"
#include "dao/DaoTemplate.h"
#include "bean/User.h"
#include "util/SqlUtil.h"
#include <QMap>
#include <QList>

const char * const SQL_NAMESPACE_USER = "User";

User UserDao::selectById(int id) {
    QString sql = SqlUtil::getInstance().getSql(SQL_NAMESPACE_USER, "selectById").arg(id);
    return DaoTemplate::selectBean(sql, mapToUser);
}

QList<User> UserDao::selectAll() {
    QString sql = SqlUtil::getInstance().getSql(SQL_NAMESPACE_USER, "selectAll");
    return DaoTemplate::selectBeans(sql, mapToUser);
}

int UserDao::insert(const User& user) {
    QString sql = SqlUtil::getInstance().getSql(SQL_NAMESPACE_USER, "insert");

    QMap<QString, QVariant> params;
    params["username"] = user.username;
    params["password"] = user.password;

    return DaoTemplate::insert(sql, params);
}

bool UserDao::update(const User& user) {
    QString sql = SqlUtil::getInstance().getSql(SQL_NAMESPACE_USER, "update");

    QMap<QString, QVariant> params;
    params["id"]       = user.id;
    params["username"] = user.username;
    params["password"] = user.password;

    return DaoTemplate::update(sql, params);
}

User UserDao::mapToUser(const QMap<QString, QVariant> &rowMap) {
    User user;
    user.id = rowMap.value("id").toInt();
    user.username = rowMap.value("username").toString();
    user.password = rowMap.value("password").toString();

    return user;
}
