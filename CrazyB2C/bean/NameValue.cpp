#include "NameValue.h"

NameValue::NameValue() {
    id = -1;
}

QString NameValue::toString() const {
    return QString("ID: %1, Name: %2, Value: %3").arg(id).arg(name).arg(value);
}
