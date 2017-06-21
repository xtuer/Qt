#ifndef NAMEVALUEBEAN_H
#define NAMEVALUEBEAN_H

#include <QString>

class NameValue {
public:
    NameValue();

    int id;
    QString name;
    QString value;

    QString toString() const;
};

#endif // NAMEVALUEBEAN_H
