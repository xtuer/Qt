#ifndef USERDAO_H
#define USERDAO_H

#include <QList>
#include <QString>
#include <QVariant>
#include <QVariantMap>

class User;

class UserDao {
public:
    static User findByUserId(int id);
    static QList<User> findtAll();

    static int  insert(const User &user);
    static bool update(const User &user);

private:
    static User mapToUser(const QVariantMap &rowMap);
};

#endif // USERDAO_H
