#ifndef USER_H
#define USER_H

#include <QString>

class User {
public:
    User();

    int id;
    QString username;
    QString password;
    QString email;
    QString mobile;

    QString toString() const;
};

#endif // USER_H
