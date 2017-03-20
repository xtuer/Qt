#include "User.h"

User::User(int id, const QString &username, const QString &password)
    : id(id), username(username), password(password) {
}

QDataStream& operator<<(QDataStream &stream, const User &user) {
    stream << user.id << user.username << user.password;
    return stream;
}

QDataStream& operator>>(QDataStream &stream, User &user) {
    stream >> user.id >> user.username >> user.password;
    return stream;
}
