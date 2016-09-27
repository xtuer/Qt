#ifndef PERSON_H
#define PERSON_H

#include <QtCore>
#include <QString>

struct PERSONINFOW;

class Person {
public:
    Person();
    Person(const PERSONINFOW &p);

    QString name;
    QString gender;
    QString nationality;
    QString birthday;
    QString address;
    QString cardId;
    QString police;
    QString validStart;
    QString validEnd;
};
Q_DECLARE_METATYPE(Person)

#endif // PERSON_H
