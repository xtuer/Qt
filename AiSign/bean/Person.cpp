#include "Person.h"
#include "lib/CardReaderApi.h"

Person::Person() {

}

Person::Person(const PERSONINFOW &p) {
    name        = QString::fromWCharArray(p.name);
    gender      = QString::fromWCharArray(p.gender);
    nationality = QString::fromWCharArray(p.nationality);
    birthday    = QString::fromWCharArray(p.birthday);
    address     = QString::fromWCharArray(p.address);
    cardId      = QString::fromWCharArray(p.cardId);
    police      = QString::fromWCharArray(p.police);
    validStart  = QString::fromWCharArray(p.validStart);
    validEnd    = QString::fromWCharArray(p.validEnd);
}

// qRegisterMetaType("Person");
