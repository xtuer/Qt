#ifndef PERSON_H
#define PERSON_H

#include <QDebug>
#include <QString>

class Person {
public:
    Person();

    friend QDebug operator<<(QDebug stream, const Person &p);

    int id;
    QString name;
};

#endif // PERSON_H
