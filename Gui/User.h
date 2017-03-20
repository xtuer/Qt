#ifndef USER_H
#define USER_H

#include <QString>
#include <QVariant>
#include <QMetaType>
#include <QDataStream>

class User {
public:
    User(int id = 50, const QString &username = QString(), const QString &password = QString());

    friend QDataStream& operator<<(QDataStream &stream, const User &user);
    friend QDataStream& operator>>(QDataStream &stream, User &user);

    int id;
    QString username;
    QString password;
};

Q_DECLARE_METATYPE(User)

#endif // USER_H
