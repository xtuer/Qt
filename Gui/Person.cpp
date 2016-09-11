#include "Person.h"

Person::Person() {

}

QDebug operator<<(QDebug stream, const Person &p) {
    stream << "ID: " << p.id << ", Name: " << p.name;

    return stream;
}
